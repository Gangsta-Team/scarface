#include "main.hpp"

#include <enet/enet.h>
#include "network_client.hpp"

int main() {
    if (enet_initialize() != 0) {
        std::cerr << "Failed to initialize ENet!" << std::endl;
        return EXIT_FAILURE;
    }
    
    ENetAddress address;
    enet_address_set_host(&address, "localhost");
    address.port = 1234;
    
    ENetHost* client = enet_host_create(nullptr, 1, 0, 0, 0);
    if (client == nullptr) {
        std::cerr << "Failed to create ENet client!" << std::endl;
        enet_deinitialize();
        return EXIT_FAILURE;
    }
    
    ENetPeer* peer = enet_host_connect(client, &address, 1, 0);
    if (peer == nullptr) {
        std::cerr << "Failed to connect to server!" << std::endl;
        enet_host_destroy(client);
        enet_deinitialize();
        return EXIT_FAILURE;
    }
    
    ENetEvent event;
    while (enet_host_service(client, &event, 1000) > 0) {
        switch (event.type) {
            case ENET_EVENT_TYPE_CONNECT:
                std::cout << "Connected to server!" << std::endl;
                break;
            case ENET_EVENT_TYPE_RECEIVE:
                std::cout << "Received packet of length " << event.packet->dataLength << std::endl;
                enet_packet_destroy(event.packet);
                break;
            case ENET_EVENT_TYPE_DISCONNECT:
                std::cout << "Disconnected from server!" << std::endl;
                return EXIT_SUCCESS;
        }
    }
    
    enet_peer_reset(peer);
    enet_host_destroy(client);
    enet_deinitialize();
    return EXIT_FAILURE;
}
