#include "mod.hpp"
#include "pointers.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "hooks.hpp"
#include "gameEventPools.hpp"

#include <enet/enet.h>
#include "network_client.hpp"

using namespace gangsta::sf_net;

namespace gangsta
{

    Client* client = nullptr;

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
        client->Update();
    }

    // Called often in Thread
    void CMod::RunLoop()
    {
        
    }

    // No cleanup or anything because its a onetime load mod :-)

}