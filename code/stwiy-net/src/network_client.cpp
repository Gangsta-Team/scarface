#include "network_client.hpp"

namespace gangsta::sf_net {
    Client::Client() {
        // Initialize ENet
        if (enet_initialize() != 0) {
            std::cerr << "Could not initialize ENet." << std::endl;
            return;
        }

        // Create the client host
        client_ = enet_host_create(NULL, 1, 2, 0, 0);

        if (client_ == NULL) {
            std::cerr << "Could not create client." << std::endl;
            return;
        }
    }

    Client::~Client() {
        thread_.join();
        enet_peer_disconnect(peer_, 0);
        enet_host_destroy(client_);
        enet_deinitialize();
    }

    void Client::Connect(const char* host, int port) {
        // Connect to the server
        ENetAddress address;
        enet_address_set_host(&address, host);
        address.port = port;

        peer_ = enet_host_connect(client_, &address, 2, 0);

        if (peer_ == NULL) {
            std::cerr << "Could not connect to server." << std::endl;
            return;
        }

        m_running = true;
        // Create a thread to handle incoming messages
        thread_ = std::thread(&Client::Update, this);

        // start the packet sender
        SendPackets();
    }

    void Client::Disconnect() {
        
    }

    void Client::SetInterface(ClientInterface* sv_interface) {
        m_interface = sv_interface;
    }

    void Client::Send(const void* data, size_t dataSize, enet_uint32 flags) {
        ENetPacket* packet = enet_packet_create(data, dataSize, flags);
        enet_peer_send(peer_, 0, packet);
        enet_host_flush(client_);
    }

    void Client::SendPacket(const RPCPacket& syncPacket) {
        ENetPacket* packet = enet_packet_create(&syncPacket, sizeof(RPCPacket), ENET_PACKET_FLAG_RELIABLE);
        enet_peer_send(peer_, 0, packet);
        enet_host_flush(peer_->host);
    }

    // function to create and enqueue a packet from another thread
    void Client::CreatePacket(const RPCPacket& syncPacket) {
        // Enqueue the RPCPacket into the concurrent queue
        Packet packet;
        packet.data = reinterpret_cast<const enet_uint8*>(&syncPacket);
        packet.dataSize = sizeof(RPCPacket);
        packet.flags = ENET_PACKET_FLAG_RELIABLE;
        packetQueue.enqueue(packet);
    }
    // function to send packets from the queue
    void Client::SendPackets() {
        while (true) {
            // try to dequeue a packet from the queue
            Packet packet;
            if (packetQueue.try_dequeue(packet)) {
                // create a new ENet packet from the dequeued packet data
                ENetPacket* enetPacket = enet_packet_create(packet.data, packet.dataSize, packet.flags);
                // send the packet using the ENet client
                enet_peer_send(peer_, 0, enetPacket);
                // flush the host to send any pending packets
                enet_host_flush(client_);
            }
            // add a small delay to prevent busy waiting
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }

    void Client::Update() {
        ENetEvent event;
        while(true){
            while (enet_host_service(client_, &event, 1000) > 0) {
                switch (event.type) {
                    case ENET_EVENT_TYPE_CONNECT:
                        if (m_interface) {
                            m_interface->onClientConnect(event);
                        }
                        break;

                    case ENET_EVENT_TYPE_DISCONNECT:
                        if (m_interface) {
                            m_interface->onClientDisconnect(event);
                        }
                        break;

                    case ENET_EVENT_TYPE_RECEIVE:
                        if (m_interface) {
                            m_interface->onPacketReceived(event);
                            enet_packet_destroy(event.packet);
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }
};
