#include "DllMain.h"

//Patterns
#include "Patterns.h"

CINI*                   gINI;
CLog*                   gLog;
CConsole*               gConsole;

int                     g_iDebugConsole = 0;
int                     g_iLog = 0;

HMODULE                 g_hGameBase = NULL;
DWORD	                g_dwImageBase = 0x40000;
HWND                    hWindow = NULL;
bool                    bRenderInit = false;
bool                    g_bRenderMenu = false;
DWORD*                  dw_CodeBlockinstance = NULL;
D3DPRESENT_PARAMETERS*  g_pPresentationParameters;

// Dx9 EndScene
void* pEndScene = NULL;
void* pDeviceTable[119];
LPDIRECT3DDEVICE9       pDxDevice9 = (LPDIRECT3DDEVICE9)NULL;

//Torque Script.
compileExec_t OriginalCodeBlock_compileExec;

char* __fastcall CodeBlock_compileExec_Hook(void* thisptr, void*, char* Str, char* Source, char* Args) {
    char* res = OriginalCodeBlock_compileExec(thisptr, Str, Source, Args);
    return res;
}

//_DWORD *__thiscall CodeBlock::ctor(_DWORD *this) CodeBlock::ctor 
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

    //bool hooked = InstallEndScene_Hook();
    //printf("End Scene hook: %d\n", hooked);

    auto pattern1002 = hook::pattern("50 6A 01 57 51 FF 52 40").count_hint(1);
    if (pattern1002.size() == 1)
    {
        printf("[INFO]: Game version: 1.00.2\n");
        
        DetourFunction((PBYTE)pattern1002.get_first(0), (PBYTE)CreateD3D9DeviceHook);
        CreateD3D9DeviceRET = pattern1002.get_first(+8);
        
            
        //InjectHook(pattern1002.get_first(0), CreateD3D9DeviceHook, PATCH_JUMP);
        //CreateD3D9DeviceRET = pattern1002.get_first(+8);
    }
    return res;
}

//00457160
typedef char(__cdecl* CreateGameWindow_t)(UINT a1);
CreateGameWindow_t OriginalCreateGameWindow;
char __cdecl CreateGameWindow_Hook(UINT a1) {
    printf("CreateGameWindow_Hook()\n");
    return OriginalCreateGameWindow(a1);
}

static HRESULT __stdcall OnCreateD3D9Device(
    IDirect3D9* pDirect3D9,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    IDirect3DDevice9** ppReturnedDeviceInterface)
{
    HRESULT hResult = pDirect3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    if (hResult != D3D_OK)
        return hResult;

    Direct3DDevice9Proxy* pDeviceProxy = new Direct3DDevice9Proxy(*ppReturnedDeviceInterface);
    *ppReturnedDeviceInterface = pDeviceProxy;

    printf("[INFO] Game resolution: \n\t - Width: %d\n\t - Height: %d\n", pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);

    g_pPresentationParameters = pPresentationParameters;
    hWindow = hFocusWindow;

    /*lpPrevWndProc = (WNDPROC)GetWindowLong(hFocusWindow, GWL_WNDPROC);
    SetWindowLong(hFocusWindow, GWL_WNDPROC, (LONG)NewWndProc);*/

        // Set mouse hook
        /*hHook = SetWindowsHookEx(
            WH_MOUSE_LL,
            mouseHookProc,
            GetModuleHandle(NULL),
            NULL);*/

    return D3D_OK;
}

static void* CreateD3D9DeviceRET;
static void __declspec(naked) CreateD3D9DeviceHook()
{
    _asm push eax
    _asm push 1
    _asm push edi
    _asm push ecx
    _asm call OnCreateD3D9Device
    _asm jmp CreateD3D9DeviceRET
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

