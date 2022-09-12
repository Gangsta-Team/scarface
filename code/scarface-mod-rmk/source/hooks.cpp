#include "hooks.hpp"
#include "pointers.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "config.hpp"
#include "common.hpp"

#include "utils/StackwalkerUtils.hpp"

#include "gameutils/radLoadBaseStreamImpl.hpp"

// UTIL

template<typename DORIG>
static inline DORIG HookFunc(void* ptr, void* detour)
{

    void* orig = nullptr;

    MH_CreateHook(ptr, detour, &orig);
    MH_EnableHook(ptr);

    return (DORIG)orig;

}

namespace gangsta
{

    int CHooks::pddiCreate(int versionMajor, int versionMinor, class pddiDevice** device)
    {

        Logger::GetInstance()->Info("pddiCreate: [ versionMajor: %i ] [ versionMinor: %i ] [ device: %p ]", versionMajor, versionMinor, device);
        Logger::GetInstance()->Info("pddiCreate: [ return device: %p ]", (device != nullptr ? *device : nullptr));

        auto pattern1002 = Signature("50 6A 01 57 51 FF 52 40").Scan().As<void*>();
        if (pattern1002)
        {
            Logger::GetInstance()->Info("Game version: 1.00.2");
        }
        else
        {
            Logger::GetInstance()->Error("Version is not 1.00.2!");
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

        Logger::GetInstance()->Info("Direct3DCreate9: [ sdkVersion: %08X ] [ device: %p ]", sdkVer, res);

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
            Logger::GetInstance()->Info("CodeBlock_compileExec_Hook(%s, %s, %s);", Str, Source, Args);
            Logger::GetInstance()->Info("\tcodeSize 0x%x", codeBlock->m_code_size);
            Logger::GetInstance()->Info("\tcode 0x%p", codeBlock->m_code);
            if (codeBlock->m_code) {
                for (uint32_t i = 0; i < codeBlock->m_code_size; ++i) {
                    uint8_t c = ((uint8_t*)codeBlock->m_code)[i];
                    Logger::GetInstance()->Info("%02X ", c);
                }
                Logger::GetInstance()->Info("\n");
                torque3d::CodeBlock::dumpInstructions(codeBlock, 0, false);
            }
            else {
                Logger::GetInstance()->Info("code nullptr");
            }
        }
        
        return res;
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

    bool CHooks::TestFunc(void*, int arg_count, char ** arg_text)
    {
        Logger::GetInstance()->Info("Called Test Function! Arg Count: %i", arg_count);
        return 1;
    }

    bool CHooks::NativeHook_Echo(void*, int _arg_count, char** arg_text)
    {
        int arg_count = _arg_count - 1;

        Logger::GetInstance()->Info("[Echo (%i)] %s: %s", arg_count - 1, arg_text[1], arg_text[2]);

        return 1;
    }

    bool CHooks::NativeHook_SlowEcho(void*, int _arg_count, char** arg_text)
    {
        Logger::GetInstance()->Info("[SlowEcho] %s: %s", arg_text[1], arg_text[2]);

        return 1;
    }

    bool CHooks::NativeHook_WarningMsg(void*, int _arg_count, char** arg_text)
    {
        int arg_count = _arg_count - 1;

        Logger::GetInstance()->Info("[WarningMsg (%i)] %s: %s", arg_count - 1, arg_text[1], arg_text[2]);

        return 1;
    }

    bool CHooks::NativeHook_DebugMsg(void*, int _arg_count, char** arg_text)
    {
        int arg_count = _arg_count - 1;

        Logger::GetInstance()->Info("[DebugMsg (%i)] %s: %s", arg_count - 1, arg_text[1], arg_text[2]);

        return 1;
    }

    bool CHooks::NativeHook_AssertMsg(void*, int _arg_count, char** arg_text)
    {
        int arg_count = _arg_count - 1;

        Logger::GetInstance()->Info("[AssertMsg (%i)] %s: %s", arg_count - 1, arg_text[1], arg_text[2]);

        return 1;
    }

    void CHooks::AssignRegisteredMethodsToNamespaces()
    {
        Logger::GetInstance()->Info("[Con::Init] Loading Registered Weapons to Namespaces!");

        {
           
            con::RegisteredMethod* testMethod = new con::RegisteredMethod(1, 1, nullptr, "GangstaTestFunc");
            testMethod->PutInList(g_Pointers.m_smRegisteredMethods);
            testMethod->func_return_void = TestFunc;

        }

        {

            con::RegisteredMethod* currentMethod = (*g_Pointers.m_smRegisteredMethods);

            if(currentMethod != nullptr)
            {
                while(currentMethod != nullptr)
                {
                    if(strcmp(currentMethod->method_name, "Echo") == 0)
                    {
                        currentMethod->func_return_void = NativeHook_Echo;
                    }

                    if(strcmp(currentMethod->method_name, "SlowEcho") == 0)
                    {
                        currentMethod->func_return_void = NativeHook_SlowEcho;
                    }

                    if(strcmp(currentMethod->method_name, "WarningMsg") == 0)
                    {
                        currentMethod->func_return_void = NativeHook_WarningMsg;
                    }

                    if(strcmp(currentMethod->method_name, "DebugMsg") == 0)
                    {
                        currentMethod->func_return_void = NativeHook_DebugMsg;
                    }

                    if(strcmp(currentMethod->method_name, "AssertMsg") == 0)
                    {
                        currentMethod->func_return_void = NativeHook_AssertMsg;
                    }

                    currentMethod = currentMethod->flink;
                }
            }

        }

        static_cast<decltype(&AssignRegisteredMethodsToNamespaces)>(g_Hooks.OriginalAssignRegisteredMethodsToNamespaces)();

        Logger::GetInstance()->Info("[Con::Init] Finished Loading Namespaces!");
    }

    int CHooks::ScriptFileChunkLoader__LoadObject(void *_this, void* edx, core::IRefCount **object, uint32_t *name, tChunkFile *file, void *a5)
    {


        auto res = static_cast<decltype(&ScriptFileChunkLoader__LoadObject)>(g_Hooks.OriginalScriptFileChunkLoaderLoadObject)(_this, edx, object, name, file, a5);

        return res;
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
    }

    void CHooks::Hook()
    {

    }

}