#include "main.hpp"
#include "mod.hpp"
#include "logger.hpp"
#include "config.hpp"

#include <MinHook.h>

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved ) // reserved
{
    
    if(fdwReason == DLL_PROCESS_ATTACH)
    {

        MH_Initialize();
        
        gangsta::g_Config.Load();
        gangsta::Logger::Init();
        gangsta::Logger::Info("Loading!");
        gangsta::g_Mod.InitSafe();

        CreateThread(nullptr, 0, [] (void*) -> DWORD
        {
            
            gangsta::g_Mod.Init();

            gangsta::Logger::Info("Welcome!");

            while (true)
            {
                gangsta::g_Mod.RunLoop();
                std::this_thread::yield();
            }
            
            return 0;

        }, nullptr, 0, nullptr);

    }

    return TRUE;
}