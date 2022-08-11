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
    public:
        // ORIGINALS HERE
        PVOID OriginalPddiCreate;
        PVOID OriginalDirect3DCreate9;
    public:
        std::map<void*, Direct3DProxy*> D3D9ProxyPool;
        std::map<void*, Direct3DDevice9Proxy*> D3D9DeviceProxyPool;
    };

    inline CHooks g_Hooks;
}

#endif