#include "hooks.hpp"
#include "pointers.hpp"
#include "logger.hpp"
#include "memory.hpp"

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

    void CHooks::HookSafe()
    {
        g_Hooks.OriginalPddiCreate = HookFunc<CPointers::PddiCreate_t>(g_Pointers.m_pddiCreate, CHooks::pddiCreate);
    }

    void CHooks::Hook()
    {

    }

}