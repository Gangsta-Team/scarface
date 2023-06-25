#include "main.hpp"

#include <iostream>
#include <enet/enet.h>
#include "packets.hpp"

// Define a function to create and populate an OnFootSyncPacket
OnFootSyncPacket CreateOnFootSyncPacket(float posX, float posY, float posZ,
                                        float rotationPitch, float rotationYaw, float rotationRoll,
                                        int animationState, int equipmentState) {
    OnFootSyncPacket syncPacket;
    syncPacket.posX = posX;
    syncPacket.posY = posY;
    syncPacket.posZ = posZ;
    syncPacket.rotationPitch = rotationPitch;
    syncPacket.rotationYaw = rotationYaw;
    syncPacket.rotationRoll = rotationRoll;
    syncPacket.animationState = animationState;
    syncPacket.equipmentState = equipmentState;
    return syncPacket;
}

void SendPacket(ENetPeer* peer, const RPCPacket& syncPacket) {
    ENetPacket* packet = enet_packet_create(&syncPacket, sizeof(RPCPacket), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(peer->host);
}

int main() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet!" << std::endl;
        return EXIT_FAILURE;
    }
    ENetHost* client = enet_host_create(nullptr, 1, 1, 0, 0);
    if (client == nullptr) {
        std::cerr << "Failed to create ENet client!" << std::endl;
        enet_deinitialize();
        return EXIT_FAILURE;
    }
    
    ENetAddress address;
    enet_address_set_host(&address, "127.0.0.1");
    address.port = 1234;

    ENetPeer* peer = enet_host_connect(client, &address, 1, 0);
    if (peer == nullptr) {
        std::cerr << "Failed to connect to server!" << std::endl;
        enet_host_destroy(client);
        enet_deinitialize();
        return EXIT_FAILURE;
    }

    ENetEvent event;
    while(true)
	{
        while (enet_host_service(client, &event, 1000) > 0) {
            switch (event.type) {
                case ENET_EVENT_TYPE_CONNECT:
                    std::cout << "Connected to server!" << std::endl;
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    std::cout << "Received packet of length " << event.packet->dataLength << std::endl;
                    if (event.packet->dataLength == sizeof(OnFootSyncPacket)) {
                        std::cout << "\t OnFootSyncPacket" << std::endl;
                        //OnFootSyncPacket syncPacket;
                        //memcpy(&syncPacket, event.packet->data, sizeof(OnFootSyncPacket));
                    }
                    enet_packet_destroy(event.packet);
                    break;
                }
                case ENET_EVENT_TYPE_DISCONNECT:
                    std::cout << "Disconnected from server!" << std::endl;
                    return EXIT_SUCCESS;
            }
        }

        OnFootSyncPacket onFootSync = CreateOnFootSyncPacket(100.0f, 100.0f, 100.0f, 50.0f, 50.0f, 50.0f, 1, 2);
        RPCPacket rpcPacket;
        rpcPacket.action = RPCAction::Walk;
        rpcPacket.playerId = 0;
        rpcPacket.onFootSync = onFootSync;

        SendPacket(peer, rpcPacket);

        Sleep(1000);
    }

    std::cout << "Press any key to exit..." << std::endl;
    std::cin.get();
    
    enet_peer_reset(peer);
    enet_host_destroy(client);
    enet_deinitialize();
    return EXIT_FAILURE;
}
