#ifndef _NETWORK_CLIENT_HPP
#define _NETWORK_CLIENT_HPP

#include "common.hpp"
#include <concurrentqueue.h>
#include "packets.hpp"

namespace gangsta::sf_net {

    struct Packet {
        const void* data;
        size_t dataSize;
        enet_uint32 flags;
    };

    class ClientInterface {
    public:
        virtual void onClientConnect(ENetEvent event) {}
        virtual void onClientDisconnect(ENetEvent event) {}
        virtual void onPacketReceived(ENetEvent event) {}
    };

    class Client {
    public:

        // create a concurrent queue to hold packets
        moodycamel::ConcurrentQueue<Packet> packetQueue;
        Client();
        ~Client();

        void Connect(const char* host, int port);
        void Disconnect();

        void SetInterface(ClientInterface* sv_interface);
        void Update();
        void CreatePacket(const RPCPacket& syncPacket);
        void SendPackets();
        void Send(const void* data, size_t dataSize, enet_uint32 flags = ENET_PACKET_FLAG_RELIABLE);
        void SendPacket(const RPCPacket& syncPacket);
    private:
        ENetHost* client_;
        ENetPeer* peer_;
        std::thread thread_;
        ClientInterface* m_interface;
        bool m_running = false;
    };
};

#endif