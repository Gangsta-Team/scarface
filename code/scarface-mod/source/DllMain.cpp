#include "DllMain.h"

CINI*           gINI;
CLog*           gLog;
CConsole*       gConsole;

int             g_iDebugConsole = 0;
int             g_iLog = 0;

HMODULE         g_hGameBase = NULL;
DWORD	        g_dwImageBase = 0x40000;

//Torque Script.
typedef char* (__thiscall* compileExec_t)(void* unk0, char* Str, char* Source, char* Args);
compileExec_t OriginalCodeBlock_compileExec;
DWORD* obj;

char* __fastcall CodeBlock_compileExec_Hook(void* thisptr, void*, char* Str, char* Source, char* Args) {
    char* res = OriginalCodeBlock_compileExec(thisptr, Str, Source, Args);
    return res;
}

void InstallPatches() {

    uint8_t* gReleaseMode = (uint8_t*)0x007C1C54;
    //uint8_t* gFinalMode = (uint8_t*)0x007C1C55;

    int iReleaseMode = gINI->ReadSetting("ReleaseMode");

    if(iReleaseMode == 0)
        *gReleaseMode = 0;
    //*gFinalMode = 0;
}

void InstallHooks() {
    OriginalCodeBlock_compileExec = (compileExec_t)DetourFunction((PBYTE)0x0490390, (PBYTE)CodeBlock_compileExec_Hook);
}

void ScarfaceMod() {

    // Initialize ini settings.
    gINI = new CINI();
    gINI->Initialize();

    // Read ini settings.
    g_iDebugConsole = gINI->ReadSetting("DebugConsole");
    g_iLog = gINI->ReadSetting("Log");

    // Initialize debug console.
    gConsole = new CConsole();
    if (g_iDebugConsole == 1)
        gConsole->Initialize();
    
    printf(MOD_NAME);

    // Initialize Log.
    gLog = new CLog();
    if (g_iLog == 1)
        gLog->Initialize();
    

    printf("Game base address: 0x%p\n", g_hGameBase);

    InstallPatches();

    // Install hooks.
    //InstallHooks();
}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        DisableThreadLibraryCalls(hinstDLL);

        g_hGameBase = GetModuleHandle(NULL);

        ScarfaceMod();

        //gLog->LogMessage(MessageType::WARNING_MSG, "Warning example");
        //gLog->LogMessage(MessageType::ERROR_MSG, "Error example");
        //gLog->LogMessage(MessageType::INFO_MSG, "Info example");
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}