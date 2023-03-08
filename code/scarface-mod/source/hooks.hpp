#ifndef _HOOKS_HPP
#define _HOOKS_HPP

#include "common.hpp"
#include "d3d9/Direct3DProxy.h"
#include "d3d9/Direct3DDevice9Proxy.h"

namespace gangsta
{

    class CHooks
    {
    public:
        void HookSafe();
        void Hook();
        static void Keyhook_install(HWND wnd);
        static void Keyhook_uninstall(void);
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
        static int __fastcall ScriptFileChunkLoader__LoadObject(void *_this, void* edx, core::IRefCount **object, uint32_t *name, tChunkFile *file, void *a5);
        static BOOL __stdcall ShowGameWindow();
        static char __cdecl ScriptLoadCompiled(char *scriptPath, char *a2, char *Str2, int a4, int a5);
        static int __cdecl Con_printf(char *Format, ...);
        static int* __fastcall CodeBlock_exec(torque3d::CodeBlock* codeBlock, void* edx, unsigned int ip, const char* functionName, void* thisNamespace, unsigned int argc, void* argv, bool noCalls, const char* packageName, signed int setFrame);
        static HANDLE h_CreateMutexA(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCSTR lpName);
        static char __fastcall sub_4ED160(int _this, void* edx, int unk0, unsigned int unk1);
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
        PVOID OriginalScriptFileChunkLoaderLoadObject;
        PVOID OriginalShowGameWindow;
        PVOID OriginalScriptLoadCompiled;
        PVOID OriginalCon_printf;
        PVOID OriginalCodeBlock_exec;
        PVOID OriginalCreateMutexA;
        PVOID Originalsub_4ED160;
    public:
        std::map<void*, Direct3DProxy*> D3D9ProxyPool;
        std::map<void*, Direct3DDevice9Proxy*> D3D9DeviceProxyPool;
    };

    inline CHooks g_Hooks;
}

#endif