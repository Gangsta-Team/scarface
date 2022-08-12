#include "hooks.hpp"
#include "pointers.hpp"
#include "logger.hpp"
#include "memory.hpp"
#include "config.hpp"

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

    void CHooks::sub_6AE3F0(void* _this, void *edx, void* a3)
    {

        void* pUnk = *(void**)((uintptr_t)_this + 0x0C);

        if(pUnk == nullptr)
        {
            gangsta::Logger::GetInstance()->Info("Crash fix");
            return;
        }

        static_cast<decltype(&CHooks::sub_6AE3F0)>(g_Hooks.OriginalSub6AE3F0)(_this, edx, a3);
    }

    int CHooks::sub_65D773(void* _this, void* edx)
    {

        gangsta::Logger::GetInstance()->Info("-");

        IDirect3DDevice9* pRenderDevice = *(IDirect3DDevice9 **)(*(DWORD *)(*((DWORD *)_this + 86) + 540) + 16);
        
        if(pRenderDevice == nullptr)
        {
            Logger::GetInstance()->Info("RenderDevice is null!");
            return S_FALSE;
        }

        IDirect3DSurface9* target = 0;
        HRESULT res_target = pRenderDevice->GetRenderTarget(0, &target);

        if(target == nullptr)
        {
            Logger::GetInstance()->Info("RenderTarget is null!");
            return S_FALSE;
        }

        target->Release();

        return static_cast<decltype(&CHooks::sub_65D773)>(g_Hooks.OriginalSub65D773)(_this, edx);
    }

    void CHooks::HookSafe()
    {
        g_Hooks.OriginalPddiCreate = HookFunc<CPointers::PddiCreate_t>(g_Pointers.m_pddiCreate, CHooks::pddiCreate);
        g_Hooks.OriginalSub6AE3F0 = HookFunc<void*>((void*)0x006AE3F0, CHooks::sub_6AE3F0);
        g_Hooks.OriginalSub65D773 = HookFunc<void*>((void*)0x0065D6D0, CHooks::sub_65D773);
    }

    void CHooks::Hook()
    {

    }

}