#include "mod.hpp"
#include "pointers.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "hooks.hpp"
#include "gameEventPools.hpp"
#include "networkClient.hpp"

namespace gangsta
{

    // Called at DllMain
    void CMod::InitSafe()
    {
        g_Pointers.ScanSafe();

        if(g_Config.parsedJson["ReleaseMode"].get<bool>() == false)
        {
            *g_Pointers.m_gReleaseMode = 0;
            Logger::Info("Spoofing Release Mode!");
        }

        //*g_Pointers.m_gDevStartupMenu = 1;
        //*g_Pointers.m_gSkipFrontEnd = 1;
        //*g_Pointers.m_gSkipToStartLocation = (int)"-3409 19 -1701";
        *g_Pointers.m_gSkipFrontEnd = 1;
        g_Hooks.HookSafe();
    }

    // Called in Thread
    void CMod::Init()
    {
        g_Pointers.Scan();
        g_Hooks.Hook();
    }

    // Called often in Thread
    void CMod::RunLoop()
    {
        g_networkClient.Update();
    }

    // No cleanup or anything because its a onetime load mod :-)

}