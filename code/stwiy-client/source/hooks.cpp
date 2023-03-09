#include "hooks.hpp"
#include "pointers.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "config.hpp"
#include "common.hpp"
#include "gameEventPools.hpp"

// Globals
HWND                    g_HWND = NULL;
WNDPROC				    g_OrigWndProc;
HWND					g_OrigWnd;

// UTIL

template<typename DORIG>
static inline DORIG HookFunc(void* ptr, void* detour)
{

    void* orig = nullptr;

    MH_CreateHook(ptr, detour, &orig);
    MH_EnableHook(ptr);

    return (DORIG)orig;

}

static LRESULT CALLBACK wnd_proc(HWND wnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
    //gangsta::Logger::Info("wnd_proc({})", umsg);
    return CallWindowProc(g_OrigWndProc, wnd, umsg, wparam, lparam);
}

namespace gangsta
{

    int CHooks::pddiCreate(int versionMajor, int versionMinor, class pddiDevice** device)
    {

        Logger::Info("pddiCreate: [ versionMajor: {} ] [ versionMinor: {} ] [ device: {:08x} ]", versionMajor, versionMinor, (uint32_t)device);
        Logger::Info("pddiCreate: [ return device: {:08x} ]", (uint32_t)(device != nullptr ? *device : nullptr));

        auto pattern1002 = Signature("50 6A 01 57 51 FF 52 40").Scan().As<void*>();
        if (pattern1002)
        {
            Logger::Info("Game version: 1.00.2");
        }
        else
        {
            Logger::Error("Version is not 1.00.2!");
            ExitProcess(-1);
        }

        g_Hooks.OriginalDirect3DCreate9 = HookFunc<void*>(GetProcAddress(GetModuleHandleA("d3d9.dll"), "Direct3DCreate9"), CHooks::Direct3DCreate9);

        int res = static_cast<CPointers::PddiCreate_t>(g_Hooks.OriginalPddiCreate)(versionMajor, versionMinor, device);

        MH_DisableHook(GetProcAddress(GetModuleHandleA("d3d9.dll"), "Direct3DCreate9"));
        MH_RemoveHook(GetProcAddress(GetModuleHandleA("d3d9.dll"), "Direct3DCreate9"));

        return res;
    }

    IDirect3D9* CHooks::Direct3DCreate9(uint32_t sdkVer)
    {

        IDirect3D9* res = static_cast<decltype(&::Direct3DCreate9)>(g_Hooks.OriginalDirect3DCreate9)(sdkVer);

        Logger::Info("Direct3DCreate9: [ sdkVersion: {:08x} ] [ device: {:08x} ]", sdkVer, (uint32_t)res);

        if(res != nullptr)
        {
            Direct3DProxy *prxy = new Direct3DProxy(res);

            g_Hooks.D3D9ProxyPool.emplace(res, prxy);

            res = dynamic_cast<IDirect3D9*>(prxy);
        }

        return res;

    }

    static void __declspec(naked) Check0065D788()
    {
        static void* nullJmpLabel   = (void*)0x0065DAAB;
        static void* workingLabel   = (void*)0x0065D78E;

        __asm {
            cmp eax, 0
            je  eqlbl
            jmp NOT_EQUAL_LABEL
        }

        eqlbl:
        __asm
        {
            jmp nullJmpLabel
        }

        NOT_EQUAL_LABEL:
        __asm
        {
            mov ecx, [eax]
            lea edx, [esp+0x18]
            jmp workingLabel
        }
    }

    static void __declspec(naked) ReWrite401140()
    {
        static void* nullret_func = (void*)0x005E1BB0;
        static void* domessage_func = (void*)0x00456DA0;
        static void* getwndmessagecode_func = (void*)0x00456E50;
        static void* loc_401175 = (void*)0x00401175;
        static void* updategamewindow_func = (void*)0x00437120;
        static void* myorig_func = (void*)0x00401140;

        __asm
        {
            push eax
            call GetForegroundWindow
            cmp eax, g_MainWindow
            jne spf_pop_orig
            pop eax
            call nullret_func
            push 2
            call nullret_func
            add esp, 4
            call domessage_func
            call getwndmessagecode_func
            cmp eax, 0x12
            je spf_loc_401175
            mov ecx, esi
            call updategamewindow_func
            mov bl, al
            call nullret_func
            call nullret_func
            test bl, bl
            je spf_orig
            jmp loc_401175
        }

        spf_pop_orig:
        __asm
        {
            pop eax
            call domessage_func
            call _Thrd_yield
            jmp spf_orig
        }

        spf_loc_401175:
        __asm
        {
            jmp loc_401175
        }

        spf_orig:
        __asm
        {
            jmp myorig_func
        }
    }

    HWND CHooks::h_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
    {
        return ::CreateWindowExA(dwExStyle, lpClassName, lpWindowName, WS_OVERLAPPEDWINDOW|WS_VISIBLE, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
    }

    int CHooks::h_ShowCursor(BOOL bShow)
    {
        return TRUE;
    }

    BOOL CHooks::h_SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags)
    {
        return TRUE;
    }

    void CHooks::ControllerInput__ReadControllerInput(void *_this, void* edx, void *actionMap)
    {
        if(g_Globals.guiOpened)
        {
            return;
        }

        static_cast<decltype(&ControllerInput__ReadControllerInput)>(g_Hooks.OriginalControllerInputReadControllerInput)(_this, edx, actionMap);
    }

    void CHooks::GenericCharacterCamera__ControllerInput__Update(void *_this, void* edx, float accelThreshold, float accelTime, float decelTime, float deltaTime)
    {
        if(g_Globals.guiOpened)
        {
            return;
        }

        static_cast<decltype(&GenericCharacterCamera__ControllerInput__Update)>(g_Hooks.OriginalGenericCharacterCameraControllerInputUpdate)(_this, edx, accelThreshold, accelTime, decelTime, deltaTime);
    }

    uint32_t CHooks::GetHashFromFileName(const char *Str)
    {
        uint32_t result = static_cast<CPointers::CodeBlock_GetName_t>(g_Hooks.OriginalGetHashFromFileName)(Str);

        if ( !Str || !*Str )
        {
            return 0;
        }

        if(strlen(Str) > 0)
        {
            g_Globals.fileHashRegister[result] = std::string(Str);
        }

        return result;
    }


    char* CHooks::CodeBlock_compileExec(torque3d::CodeBlock* codeBlock, void* edx, char* Str, char* Source, char* Args) {
        char* res = static_cast<decltype(&CodeBlock_compileExec)>(g_Hooks.OriginalCodeBlock_compileExec)(codeBlock, edx, Str, Source, Args);
        return res;
    }
    int* CHooks::CodeBlock_exec(torque3d::CodeBlock* codeBlock, void* edx, unsigned int ip, const char* functionName, void* thisNamespace, unsigned int argc, void* argv, bool noCalls, const char* packageName, signed int setFrame)
    {
        Logger::Info("[CHooks::CodeBlock_exec]");
        return static_cast<decltype(&CodeBlock_exec)>(g_Hooks.OriginalCodeBlock_exec)(codeBlock, edx, ip, functionName, thisNamespace, argc, argv, noCalls, packageName, setFrame);
    }

    void CHooks::AssignRegisteredMethodsToNamespaces()
    {
        Logger::Info("[Con::Init] Loading Registered Weapons to Namespaces!");

        event_pools::gNativeEventPools.Dispatch();

        static_cast<decltype(&AssignRegisteredMethodsToNamespaces)>(g_Hooks.OriginalAssignRegisteredMethodsToNamespaces)();

        Logger::Info("[Con::Init] Finished Loading Namespaces!");
    }

    void CHooks::Keyhook_install(HWND wnd)
    {
        if (g_OrigWndProc == NULL || wnd != g_OrigWnd)
        {
            Keyhook_uninstall();
            g_OrigWndProc = (WNDPROC)(UINT_PTR)SetWindowLong(wnd, GWL_WNDPROC, (LONG)(UINT_PTR)wnd_proc);
            g_OrigWnd = wnd;
        }
    }

    void CHooks::Keyhook_uninstall(void)
    {
        if (g_OrigWnd != NULL)
        {
            SetWindowLong(g_OrigWnd, GWL_WNDPROC, (LONG)(UINT_PTR)g_OrigWndProc);
            g_OrigWnd = NULL;
        }
    }

    BOOL CHooks::ShowGameWindow()
    {
        BOOL res = ShowWindow(g_HWND, SW_SHOW);
        Keyhook_install((HWND)(*(DWORD*)0x0080FD9C));
        return res;
    }

     char CHooks::ScriptLoadCompiled(char *scriptPath, char *a2, char *Str2, int a4, int a5){

        Logger::Info("[CHooks::ScriptLoadCompiled] Script: {}", scriptPath);

        std::filesystem::path filePath = scriptPath;

        if (filePath.extension() == ".cs") {
            std::string my_str(scriptPath);
            if(my_str.rfind("script/", 0) == 0){
                std::string newPath = my_str.replace(0,6,"./scriptc");
                std::filesystem::path newScript = newPath.c_str();
                if(std::filesystem::exists(newScript)){
                    Logger::Info("[CHooks::ScriptLoadCompiled] Loading new script: {}", newPath);
                    static torque3d::CodeBlock* p_CBInstance = nullptr;
                    if (p_CBInstance == nullptr) {
                        p_CBInstance = new torque3d::CodeBlock();
                    }

                    std::ifstream t(newPath);
                    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

                    gangsta::g_Pointers.m_compileExec(p_CBInstance, NULL, (char*)str.c_str(), NULL);
                    return NULL;
                }
                return static_cast<decltype(&ScriptLoadCompiled)>(g_Hooks.OriginalScriptLoadCompiled)(scriptPath, a2, Str2, a4, a5);
            }
            return static_cast<decltype(&ScriptLoadCompiled)>(g_Hooks.OriginalScriptLoadCompiled)(scriptPath, a2, Str2, a4, a5);
        }
        return static_cast<decltype(&ScriptLoadCompiled)>(g_Hooks.OriginalScriptLoadCompiled)(scriptPath, a2, Str2, a4, a5);
    }

    char* CHooks::Con_evaluate(const char *string, bool echo, char *fileName, int a1, int a2)
    {
        Logger::Info("[CHooks::Con_evaluate] Script: {}", string);
        return static_cast<decltype(&Con_evaluate)>(g_Hooks.OriginalCon_evaluate)(string, echo, fileName, a1, a2);
    }

    void CHooks::CVM_SetMainCharacterPackage(void* _this, void* edx, char *Str2, char a4)
    {
        Logger::Info("[CHooks::CVM_SetMainCharacterPackage] Script: {}", Str2);
        return static_cast<decltype(&CVM_SetMainCharacterPackage)>(g_Hooks.OriginalCVM_SetMainCharacterPackage)(_this, edx, Str2, a4);
    }

    void CHooks::HookSafe()
    {
        if(g_Config.parsedJson["WindowedSpoof"].get<bool>())
        {
            HookFunc<void*>((void*)0x00401140, ReWrite401140);
            *((void**)0x009CE5E8) = &CHooks::h_CreateWindowExA;
            *((void**)0x009CE76C) = &CHooks::h_ShowCursor;
            *((void**)0x009CE760) = &CHooks::h_SetWindowPos;
        }

        g_Hooks.OriginalControllerInputReadControllerInput = HookFunc<void*>((void*)0x0057E310, CHooks::ControllerInput__ReadControllerInput);
        g_Hooks.OriginalGenericCharacterCameraControllerInputUpdate = HookFunc<void*>((void*)0x00565060, CHooks::GenericCharacterCamera__ControllerInput__Update);
        g_Hooks.OriginalPddiCreate = HookFunc<CPointers::PddiCreate_t>(g_Pointers.m_pddiCreate, CHooks::pddiCreate);
        g_Hooks.OriginalGetHashFromFileName = HookFunc<CPointers::CodeBlock_GetName_t>((void*)0x0042BF90, CHooks::GetHashFromFileName);
        g_Hooks.OriginalCodeBlock_compileExec = HookFunc<CPointers::CompileExec_t>((void*)0x00490390, CHooks::CodeBlock_compileExec);
        g_Hooks.OriginalAssignRegisteredMethodsToNamespaces = HookFunc<void*>((void*)0x004926D0, CHooks::AssignRegisteredMethodsToNamespaces);
        g_Hooks.OriginalShowGameWindow = HookFunc<void*>((void*)0x00456B00, CHooks::ShowGameWindow);
        g_Hooks.OriginalScriptLoadCompiled = HookFunc<void*>((void*)0x0047FFE0, CHooks::ScriptLoadCompiled);
        g_Hooks.OriginalCon_evaluate = HookFunc<void*>((void*)0x004922D0, CHooks::Con_evaluate);
        g_Hooks.OriginalCVM_SetMainCharacterPackage = HookFunc<void*>((void*)0x004EA090, CHooks::CVM_SetMainCharacterPackage);
    }

    void CHooks::Hook()
    {

    }

}