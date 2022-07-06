
#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

extern HWND					hWindow;
extern DWORD*				dw_CodeBlockinstance;

typedef char* (__thiscall* compileExec_t)(void*, char* Str, char* Source, char* Args);
extern compileExec_t		OriginalCodeBlock_compileExec;

typedef DWORD* (__thiscall* CodeBlock_ctor_t)(BYTE* thisptr);
extern CodeBlock_ctor_t		OriginalCodeBlock_ctor;
extern DWORD*				g_pCodeBlockInstance;

extern void* CreateD3D9DeviceRET;
static void CreateD3D9DeviceHook();

#define PROJECT_NAME	"scarface-mod"
#define MOD_NAME		"[-] Scarface The World Is Yours Mod [-]\n"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")

//Dx9 Proxy
#include "Direct3DDevice9Proxy.h"
// INI
#include "CINI.h"
// CLog
#include "CLog.h"
// Console
#include "CConsole.h"

// Detours lib
#pragma comment(lib, "detours.lib")
#include "detours/detours.h"

extern CINI*		gINI;
extern CLog*		gLog;
extern CConsole*	gConsole;