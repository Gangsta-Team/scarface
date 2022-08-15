#ifndef _HOOKS_HPP
#define _HOOKS_HPP

#include "common.hpp"
#include "Direct3DProxy.h"
#include "Direct3DDevice9Proxy.h"

namespace gangsta
{

    class CHooks
    {
    public:
        void HookSafe();
        void Hook();
    public:
        // STATIC HOOKS HERE
        static int __cdecl pddiCreate(int versionMajor, int versionMinor, class pddiDevice** device);
        static IDirect3D9* __stdcall Direct3DCreate9(uint32_t sdkVer);
        static void __fastcall sub_6AE3F0(void* _this, void* edx, void* a3);
        static int __fastcall sub_65D773(void* _this, void* edx); 
        static HWND __stdcall SpoofGetForegroundWindow();
        static BOOL __stdcall PeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
        static bool __fastcall UpdateGameWindow(void* _this, void* edx);

    public:
        // ORIGINALS HERE
        PVOID OriginalPddiCreate;
        PVOID OriginalDirect3DCreate9;
        PVOID OriginalSub6AE3F0;
        PVOID OriginalSub65D773;
        decltype(&::PeekMessageA) OriginalPeekMessageA;
        PVOID OriginalUpdateGameWindow;
    public:
        std::map<void*, Direct3DProxy*> D3D9ProxyPool;
        std::map<void*, Direct3DDevice9Proxy*> D3D9DeviceProxyPool;
    };

    inline CHooks g_Hooks;
}

#endif