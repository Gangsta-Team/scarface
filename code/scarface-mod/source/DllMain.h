
#include <Windows.h>
#include <iostream>

#define PROJECT_NAME	"scarface-mod"
#define MOD_NAME		"[-] Scarface The World Is Yours Mod [-]\n"

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