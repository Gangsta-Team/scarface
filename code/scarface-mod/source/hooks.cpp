#include "hooks.hpp"
#include "pointers.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "config.hpp"
#include "common.hpp"
#include "gameEventPools.hpp"

#include "utils/StackwalkerUtils.hpp"

#include "gameutils/radLoadBaseStreamImpl.hpp"
#include "gameutils/codeblockhelper.hpp"

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

        if(g_Config.parsedJson["DumpCodeBlocks"].get<bool>())
        {
            Logger::Info("[CHooks::CodeBlock_compileExec] {}", Source);
            //Logger::Info("\tcodeSize {:08x}", codeBlock->m_code_size);
            //Logger::Info("\tcode {:08x}", (uint32_t)codeBlock->m_code);
            if (codeBlock->m_code) {
                //for (uint32_t i = 0; i < codeBlock->m_code_size; ++i) {
                //    uint8_t c = ((uint8_t*)codeBlock->m_code)[i];
                //    Logger::Info("{:02x} ", c);
                //}
                //torque3d::CodeBlock::dumpInstructions(codeBlock, 0, false);
                //dumpInstructions(codeBlock, 0, false);
            }
            else {
                Logger::Info("code nullptr");
            }
        }
        
        return res;
    }
    int* CHooks::CodeBlock_exec(torque3d::CodeBlock* codeBlock, void* edx, unsigned int ip, const char* functionName, void* thisNamespace, unsigned int argc, void* argv, bool noCalls, const char* packageName, signed int setFrame)
    {
        Logger::Info("[CHooks::CodeBlock_exec]");
        return static_cast<decltype(&CodeBlock_exec)>(g_Hooks.OriginalCodeBlock_exec)(codeBlock, edx, ip, functionName, thisNamespace, argc, argv, noCalls, packageName, setFrame);
    }

    int CHooks::GenericSpawnObject__GetTotalNumToSpawn(void* _this, void* edx)
    {
        //int res = static_cast<decltype(&GenericSpawnObject__GetTotalNumToSpawn)>(g_Hooks.OriginalGenericSpawnObjectGetTotalNumToSpawn)(_this, edx);
        int res = 0;
        //Logger::GetInstance()->Info("T");

        res = 0;

        return res;
    }

    int CHooks::GenericSpawnObject__GetCoexistingCount(void* _this, void* edx)
    {
        return 0;
    }

    void CHooks::AssignRegisteredMethodsToNamespaces()
    {
        Logger::Info("[Con::Init] Loading Registered Weapons to Namespaces!");

        event_pools::gNativeEventPools.Dispatch();

        static_cast<decltype(&AssignRegisteredMethodsToNamespaces)>(g_Hooks.OriginalAssignRegisteredMethodsToNamespaces)();

        Logger::Info("[Con::Init] Finished Loading Namespaces!");
    }

    int CHooks::ScriptFileChunkLoader__LoadObject(void *_this, void* edx, core::IRefCount **object, uint32_t *name, tChunkFile *file, void *a5)
    {


        auto res = static_cast<decltype(&ScriptFileChunkLoader__LoadObject)>(g_Hooks.OriginalScriptFileChunkLoaderLoadObject)(_this, edx, object, name, file, a5);

        return res;
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

    
    int CHooks::Con_printf(char *Format, ...)
    {
        Logger::Info("[CHooks::Con_printf]");
        char Dest[1024]; // [esp+0h] [ebp-400h] BYREF
        va_list Args; // [esp+408h] [ebp+8h] BYREF

        va_start(Args, Format);
        return vsprintf(Dest, Format, Args);
    }

    HANDLE CHooks::h_CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName)
    {
        return static_cast<decltype(&h_CreateMutexA)>(g_Hooks.OriginalCreateMutexA)(lpMutexAttributes, bInitialOwner, lpName);
    }

    char CHooks::sub_4ED160(int _this, void* edx, int unk0, unsigned int unk1)
    {
        bool ret = static_cast<decltype(&sub_4ED160)>(g_Hooks.Originalsub_4ED160)(_this, edx, unk0, unk1);
        Logger::Info("[CHooks::sub_4ED160] {}", ret);
        return true;
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
        // g_Hooks.OriginalGenericSpawnObjectGetTotalNumToSpawn = HookFunc<void*>((void*)0x005F9DD0, CHooks::GenericSpawnObject__GetTotalNumToSpawn);
        // g_Hooks.OriginalGenericSpawnObjectGetCoexistingCount = HookFunc<void*>((void*)0x005F9E80, CHooks::GenericSpawnObject__GetCoexistingCount);
        g_Hooks.OriginalAssignRegisteredMethodsToNamespaces = HookFunc<void*>((void*)0x004926D0, CHooks::AssignRegisteredMethodsToNamespaces);
        g_Hooks.OriginalScriptFileChunkLoaderLoadObject = HookFunc<void*>((void*)0x00489360, CHooks::ScriptFileChunkLoader__LoadObject);
        g_Hooks.OriginalShowGameWindow = HookFunc<void*>((void*)0x00456B00, CHooks::ShowGameWindow);
        g_Hooks.OriginalScriptLoadCompiled = HookFunc<void*>((void*)0x0047FFE0, CHooks::ScriptLoadCompiled);
        g_Hooks.OriginalCon_printf = HookFunc<void*>((void*)0x004918E0, CHooks::Con_printf);
        g_Hooks.Originalsub_4ED160 = HookFunc<void*>((void*)0x004ED160, CHooks::sub_4ED160);
        //g_Hooks.OriginalCodeBlock_exec = HookFunc<void*>((void*)0x0048AB80, CHooks::CodeBlock_exec);
        //StatePropManager::CreateStateprop
        //g_Hooks.OriginalCreateMutexA = HookFunc<void*>((void*)0x009CE150, CHooks::h_CreateMutexA);
        //HookFunc<void*>((void*)0x009CE150, h_CreateMutexA);
        //*((void**)0x009CE150) = &CHooks::h_CreateMutexA;
        //HookFunc<void*>((void*)0x009CE150, h_CreateMutexA);
    }

    void CHooks::Hook()
    {

    }

}