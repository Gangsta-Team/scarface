#ifndef _NETWORK_SERVER_HPP
#define _NETWORK_SERVER_HPP

#include "common.hpp"

namespace gangsta::sf_net {
    class ServerInterface {
    public:
        virtual void onClientConnect(ENetEvent event) {}
        virtual void onClientDisconnect(ENetEvent event) {}
        virtual void onPacketReceived(ENetEvent event) {}
    };

    class Server {
    public:
        Server(int port, int maxClients);
        ~Server();

        void Start();
        void Stop();
        void Send(ENetPeer* peer, const std::string& message);

        void SetInterface(ServerInterface* sv_interface);
        void Update();

    private:
        ENetHost* server_;
        ServerInterface* m_interface;
        std::thread thread_;
        bool stop_ = false;
        bool m_running = false;
    };
};

#endif