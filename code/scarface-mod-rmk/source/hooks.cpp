#include "hooks.hpp"
#include "pointers.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "config.hpp"

#include "utils/StackwalkerUtils.hpp"

#include <MinHook.h>

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
        Logger::GetInstance()->Info("pddiCreate: [ out   device: %p ]", (device != nullptr ? *device : nullptr));

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


    void CHooks::HookSafe()
    {
        if(g_Config.parsedJson["WindowedSpoof"].get<bool>())
        {
            HookFunc<void*>((void*)0x00401140, ReWrite401140);
        }
        g_Hooks.OriginalPddiCreate = HookFunc<CPointers::PddiCreate_t>(g_Pointers.m_pddiCreate, CHooks::pddiCreate);
    }

    void CHooks::Hook()
    {

    }

}