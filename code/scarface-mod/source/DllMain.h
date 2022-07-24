
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

//Torque3D
#include "Torque3D/types.h"
#include "Torque3D/ast.h"

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

enum CompiledInstructions
{
    OP_FUNC_DECL,
    OP_CREATE_OBJECT,
    OP_2,
    OP_3,
    OP_ADD_OBJECT,
    OP_END_OBJECT,
    OP_JMPIFFNOT,
    OP_JMPIFNOT,
    OP_JMPIFF,
    OP_JMPIF,
    OP_JMPIFNOT_NP,
    OP_JMPIF_NP,
    OP_JMP,
    OP_RETURN,
    OP_CMPEQ,
    OP_CMPLT,
    OP_CMPLE,
    OP_CMPGR,
    OP_CMPGE,
    OP_CMPNE,
    OP_XOR,
    OP_MOD,
    OP_BITAND,
    OP_BITOR,
    OP_NOT,
    OP_NOTF,
    OP_ONESCOMPLEMENT,
    OP_SHR,
    OP_SHL,
    OP_AND,
    OP_OR,
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_NEG,
    OP_SETCURVAR,
    OP_SETCURVAR_CREATE,
    OP_SETCURVAR_KURVA,
    OP_SETCURVAR_ANYAD,
    OP_SETCURVAR_ARRAY,
    OP_SETCURVAR_ARRAY_CREATE,
    OP_SETCURVAR_ARRAY_DUNNOWHATISTHIS,
    OP_SETCURVAR_ARRAY_DUNNOWHATISTHIS2,
    OP_LOADVAR_UINT,
    OP_LOADVAR_FLT,
    OP_LOADVAR_STR,
    OP_SAVEVAR_UINT,
    OP_SAVEVAR_FLT,
    OP_SAVEVAR_STR,
    OP_SETCUROBJECT,
    OP_SETCUROBJECT_NEW,
    OP_SETCURFIELD,
    OP_SETCURFIELD_ARRAY,
    OP_LOADFIELD_UINT,
    OP_LOADFIELD_FLT,
    OP_LOADFIELD_STR,
    OP_SAVEFIELD_UINT,
    OP_SAVEFIELD_FLT,
    OP_SAVEFIELD_STR,
    OP_STR_TO_UINT,
    OP_STR_TO_FLT,
    OP_STR_TO_NONE,
    OP_FLT_TO_UINT,
    OP_FLT_TO_STR,
    OP_FLT_TO_NONE,
    OP_UINT_TO_FLT,
    OP_UINT_TO_STR,
    OP_UINT_TO_NONE,
    OP_LOADIMMED_UINT,
    OP_LOADIMMED_FLT,
    OP_LOADIMMED_STR,
    OP_LOADIMMED_IDENT,
    OP_TAG_TO_STR,
    OP_CALLFUNC_RESOLVE,
    OP_CALLFUNC,
    OP_ADVANCE_STR,
    OP_ADVANCE_STR_APPENDCHAR,
    OP_ADVANCE_STR_COMMA,
    OP_ADVANCE_STR_NUL,
    OP_REWIND_STR,
    OP_TERMINATE_REWIND_STR,
    OP_COMPARE_STR,
    OP_PUSH,
    OP_PUSH_FRAME,
};

enum {
    FunctionCall,
    MethodCall,
    ParentCall
};

void dumpInstructions(void* codeBlock, U32 startIp = 0, bool upToReturn = false, U32 codeSize = NULL, U16* code = NULL);