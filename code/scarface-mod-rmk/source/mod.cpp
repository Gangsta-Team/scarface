#include "mod.hpp"
#include "pointers.hpp"
#include "config.hpp"
#include "logger.hpp"
#include "hooks.hpp"

#include <pure3d.h>
#include <cement.h>

TextEditor* g_TE;

// create a file browser instance
ImGui::FileBrowser fileDialog;

namespace gangsta
{

    // Called at DllMain
    void CMod::InitSafe()
    {
        g_Pointers.ScanSafe();

        if(g_Config.parsedJson["ReleaseMode"].get<bool>() == false)
        {
            *g_Pointers.m_gReleaseMode = 0;
            Logger::GetInstance()->Info("Spoofing Release Mode!");
        }

        g_Hooks.HookSafe();
    }

    // Called in Thread
    void CMod::Init()
    {
        g_Pointers.Scan();
        g_Hooks.Hook();

        // (optional) set browser properties
        fileDialog.SetTitle("Gangster / File Browser");
        fileDialog.SetTypeFilters({ ".cs" });

        g_TE = new TextEditor();
    }

    // Called often in Thread
    void CMod::RunLoop()
    {}

    // No cleanup or anything because its a ontime load mod :-)

}