#include "network_client.hpp"

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
}

void Client::Disconnect() {
    
}

void Client::SetInterface(ClientInterface* sv_interface) {
    m_interface = sv_interface;
}

void Client::Send(const void* data, size_t dataSize, enet_uint32 flags = ENET_PACKET_FLAG_RELIABLE) {
    ENetPacket* packet = enet_packet_create(data, dataSize, flags);
    enet_peer_send(peer_, 0, packet);
    enet_host_flush(client_);
}

void Client::Update() {
    ENetEvent event;
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
