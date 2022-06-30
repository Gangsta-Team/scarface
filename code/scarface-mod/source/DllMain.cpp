#include "DllMain.h"

CINI*                   gINI;
CLog*                   gLog;
CConsole*               gConsole;

int                     g_iDebugConsole = 0;
int                     g_iLog = 0;

HMODULE                 g_hGameBase = NULL;
DWORD	                g_dwImageBase = 0x40000;
HWND                    g_hWnd = NULL;
HWND                    hWindow = NULL;
bool                    bRenderInit = false;
bool                    g_bRenderMenu = false;
DWORD*                  dw_CodeBlockinstance = NULL;

// Dx9 EndScene
void* pEndScene = NULL;
void* pDeviceTable[119];
LPDIRECT3DDEVICE9       pDxDevice9 = (LPDIRECT3DDEVICE9)NULL;

//Torque Script.
typedef char* (__thiscall* compileExec_t)(void*, char* Str, char* Source, char* Args);
compileExec_t OriginalCodeBlock_compileExec;

char* __fastcall CodeBlock_compileExec_Hook(void* thisptr, void*, char* Str, char* Source, char* Args) {
    char* res = OriginalCodeBlock_compileExec(thisptr, Str, Source, Args);
    return res;
}

//_DWORD *__thiscall CodeBlock::ctor(_DWORD *this) CodeBlock::ctor 
typedef DWORD* (__thiscall* CodeBlock_ctor_t)(BYTE* thisptr);
CodeBlock_ctor_t OriginalCodeBlock_ctor;
DWORD* g_pCodeBlockInstance;
DWORD* __fastcall  CodeBlock_ctor_Hook(BYTE* thisptr, void* unk) {
    g_pCodeBlockInstance = OriginalCodeBlock_ctor(thisptr);
    return g_pCodeBlockInstance;
}

// Game
typedef signed int (__cdecl* PddiCreate_t)(int a1, int a2, int* a3);
PddiCreate_t OriginalPddiCreate;
signed int PddiCreate_Hook(int a1, int a2, int* a3) {
    printf("PddiCreate_Hook()\n");
    int res = OriginalPddiCreate(a1, a2, a3);

    bool hooked = InstallEndScene_Hook();
    //printf("End Scene hook: %d\n", hooked);
    return res;
}

//00457160
typedef char(__cdecl* CreateGameWindow_t)(UINT a1);
CreateGameWindow_t OriginalCreateGameWindow;
char __cdecl CreateGameWindow_Hook(UINT a1) {
    printf("CreateGameWindow_Hook()\n");
    return OriginalCreateGameWindow(a1);
}

typedef HRESULT(__stdcall* EndScene_t)(LPDIRECT3DDEVICE9);
EndScene_t  OriginalEndScene = (EndScene_t)NULL;
HRESULT __stdcall EndScene_Hook(LPDIRECT3DDEVICE9 pDevice)
{
    pDxDevice9 = pDevice;

    if (GetAsyncKeyState(VK_F1) & 1) {
        if (dw_CodeBlockinstance == NULL) {
            BYTE* ayy = (BYTE*)malloc(52);
            memset(ayy, 0, 52);
            dw_CodeBlockinstance = OriginalCodeBlock_ctor(ayy);
        }
        printf("dw_CodeBlockinstance %p\n", dw_CodeBlockinstance);

        std::ifstream t("./Script.cs");
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        printf("%s", str.c_str());
        OriginalCodeBlock_compileExec(dw_CodeBlockinstance, NULL, (char*)str.c_str(), NULL);
    }

    return OriginalEndScene(pDevice);
}

bool InstallEndScene_Hook() {
    if (GetD3D9Device((void**)pDeviceTable, 119))
    {
        pEndScene = pDeviceTable[42];
        OriginalEndScene = (EndScene_t)DetourFunction((PBYTE)pEndScene, (PBYTE)EndScene_Hook);

        return true;
    }
    return false;
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
    OriginalCreateGameWindow = (CreateGameWindow_t)DetourFunction((PBYTE)0x00457160, (PBYTE)CreateGameWindow_Hook);
    OriginalPddiCreate = (PddiCreate_t)DetourFunction((PBYTE)0x007035B0, (PBYTE)PddiCreate_Hook);
    OriginalCodeBlock_compileExec = (compileExec_t)DetourFunction((PBYTE)0x0490390, (PBYTE)CodeBlock_compileExec_Hook);
    OriginalCodeBlock_ctor = (CodeBlock_ctor_t)DetourFunction((PBYTE)0x48CFE0, (PBYTE)CodeBlock_ctor_Hook);
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

  /*  typedef HWND GetWindowHWND(void);
    GetWindowHWND* f = (GetWindowHWND*)0x00456AF0;*/

    InstallPatches();

    // Install hooks.
    InstallHooks();

  /*  while (true) {
        if (GetAsyncKeyState(VK_INSERT) & 1) {
            HWND i = f();

            printf("Game window HWND: 0x%p\n", i);
        }
    }*/
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
        //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ScarfaceMod, 0, 0, 0);

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

