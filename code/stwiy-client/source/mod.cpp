#include "mod.hpp"
#include "pointers.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "hooks.hpp"
#include "gameEventPools.hpp"

Client* client = nullptr;

namespace gangsta
{
    struct PACKET_PING{
        std::string ping;
    };

    class ClientEvents : public ClientInterface {
    public:
        void onClientConnect(ENetEvent event) override {
            std::cout << "[CLIENT]::Client connected: " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
        }

        void onClientDisconnect(ENetEvent event) override {
            std::cout << "[CLIENT]::Client disconnected: " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
        }

        void onPacketReceived(ENetEvent event) override {
            std::cout << "[CLIENT]::Packet received from client: " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
        }
    };


    // Called at DllMain
    void CMod::InitSafe()
    {
        g_Pointers.ScanSafe();

        if(g_Config.parsedJson["ReleaseMode"].get<bool>() == false)
        {
            *g_Pointers.m_gReleaseMode = 0;
            Logger::Info("Spoofing Release Mode!");
        }

        *g_Pointers.m_gSkipFrontEnd = 1;
        g_Hooks.HookSafe();
    }

    // Called in Thread
    void CMod::Init()
    {
        g_Pointers.Scan();
        g_Hooks.Hook();
        
        client = new Client();
        ClientEvents ce;
        client->SetInterface(&ce);

        client->Connect("localhost", 1234);
    }

    // Called often in Thread
    void CMod::RunLoop()
    {
        //PACKET_PING packet_ping;
        //packet_ping.ping = "ping";
        //client->Send(&packet_ping, sizeof(packet_ping), ENET_PACKET_FLAG_RELIABLE);

        // create a packet from another thread and enqueue it
        //client->CreatePacket("Hello, ENet!", 13, 0);
    }

    // No cleanup or anything because its a onetime load mod :-)

}