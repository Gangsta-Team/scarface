#include "network_server.hpp"

Server::Server(int port, int maxClients) {
    if (enet_initialize() != 0) {
        std::cout << "Failed to initialize ENet" << std::endl;
    }

    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;

    server_ = enet_host_create(&address, maxClients, 2, 0, 0);
    if (server_ == nullptr) {
        std::cout << "Failed to create server host" << std::endl;
    }

    std::cout << "Server running is running!" << std::endl
              << "- Host: " << address.host << std::endl
              << "- Port: " << address.port << std::endl;
}

Server::~Server() {
    enet_host_destroy(server_);
    enet_deinitialize();
}

void Server::SetInterface(ServerInterface* sv_interface) {
    m_interface = sv_interface;
}

void Server::Start() {
    if (m_running) {
        return;
    }

    m_running = true;
    thread_ = std::thread(&Server::Update, this);
}

void Server::Stop() {
    if (!m_running) 
        std::cout << "Server is not running" << std::endl;

    stop_ = true;
    //thread_.join();
    //thread_.reset();
    stop_ = false;
    m_running = false;
}

void Server::Send(ENetPeer* peer, const std::string& message) {
    ENetPacket* packet = enet_packet_create(message.c_str(), message.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
}

void Server::Update() {
    ENetEvent event;
    while (enet_host_service(server_, &event, 0) > 0) {
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