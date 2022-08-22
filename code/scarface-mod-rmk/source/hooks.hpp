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
        static HWND __stdcall h_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam);
        static int __stdcall h_ShowCursor(BOOL bShow);
        static BOOL __stdcall h_SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
        static void __fastcall ControllerInput__ReadControllerInput(void *_this, void* edx, void *actionMap);
        static void __fastcall GenericCharacterCamera__ControllerInput__Update(void *_this, void* edx, float accelThreshold, float accelTime, float decelTime, float deltaTime);
        static uint32_t __cdecl GetHashFromFileName(const char *Str);
        static char* __fastcall CodeBlock_compileExec(torque3d::CodeBlock* codeBlock, void* edx, char* Str, char* Source, char* Args);
        static int __fastcall GenericSpawnObject__GetTotalNumToSpawn(void* _this, void* edx);
        static int __fastcall GenericSpawnObject__GetCoexistingCount(void* _this, void* edx);
        static void __stdcall AssignRegisteredMethodsToNamespaces();
    public:
        // ORIGINALS HERE
        PVOID OriginalPddiCreate;
        PVOID OriginalDirect3DCreate9;
        PVOID OriginalControllerInputReadControllerInput;
        PVOID OriginalGenericCharacterCameraControllerInputUpdate;
        PVOID OriginalGetHashFromFileName;
        PVOID OriginalCodeBlock_compileExec;
        PVOID OriginalGenericSpawnObjectGetTotalNumToSpawn;
        PVOID OriginalGenericSpawnObjectGetCoexistingCount;
        PVOID OriginalAssignRegisteredMethodsToNamespaces;
    public:
        std::map<void*, Direct3DProxy*> D3D9ProxyPool;
        std::map<void*, Direct3DDevice9Proxy*> D3D9DeviceProxyPool;
    };

    inline CHooks g_Hooks;
}

#endif