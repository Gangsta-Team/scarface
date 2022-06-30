
#include <Windows.h>
#include <iostream>

extern HWND			hWindow;

#define PROJECT_NAME	"scarface-mod"
#define MOD_NAME		"[-] Scarface The World Is Yours Mod [-]\n"

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib, "d3d9.lib")

// Helpers
#include "Helpers.h"
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

bool InstallEndScene_Hook();