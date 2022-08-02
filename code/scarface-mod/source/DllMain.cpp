#include "DllMain.h"

//Patterns
#include "Patterns.h"

CINI* gINI;
CLog* gLog;
CConsole* gConsole;

int                     g_iDebugConsole = 0;
int                     g_iLog = 0;

HMODULE                 g_hGameBase = NULL;
DWORD	                g_dwImageBase = 0x40000;
HWND                    hWindow = NULL;
bool                    bRenderInit = false;
bool                    g_bRenderMenu = false;
DWORD* dw_CodeBlockinstance = NULL;
D3DPRESENT_PARAMETERS* g_pPresentationParameters;

// Dx9 EndScene
void* pEndScene = NULL;
void* pDeviceTable[119];
LPDIRECT3DDEVICE9       pDxDevice9 = (LPDIRECT3DDEVICE9)NULL;

//Torque Script.
compileExec_t OriginalCodeBlock_compileExec;
U32 g_codeSize;
U32* g_code;

char* __fastcall CodeBlock_compileExec_Hook(CodeBlock* thisptr, void*, char* Str, char* Source, char* Args) {
    printf("CodeBlock_compileExec_Hook(%s, %s, %s);\n", Str, Source, Args);
    char* res = OriginalCodeBlock_compileExec(thisptr, Str, Source, Args);

    U32 codeSize = *(U32*)(((DWORD)thisptr) + 0x14);
    U32* code = *(U32**)(((DWORD)thisptr) + 0x18);
    printf("codeSize 0x%x\n", codeSize);
    printf("code 0x%p\n", code);
    if (code) {
        for (U32 i = 0; i < codeSize; ++i) {
            U8 c = ((U8*)code)[i];
            printf("%02X ", c);
        }
        printf("\n");
        dumpInstructions(thisptr, 0, false, codeSize, code);
    }
    else {
        printf("code nullptr\n");
    }

    return res;
}

//_DWORD *__thiscall CodeBlock::ctor(_DWORD *this) CodeBlock::ctor 
CodeBlock_ctor_t OriginalCodeBlock_ctor;
DWORD* g_pCodeBlockInstance;
DWORD* __fastcall  CodeBlock_ctor_Hook(BYTE* thisptr, void* unk) {
    g_pCodeBlockInstance = OriginalCodeBlock_ctor(thisptr);
    return g_pCodeBlockInstance;
}

//int __cdecl sub_48D1E0(_DWORD* a1, int a2, int a3, int a4, int a5)
typedef U32(__cdecl* compileBlock_t)(StmtNode* block, U32* codeStream, U32 ip, U32 continuePoint, U32 breakPoint);
compileBlock_t OriginalcompileBlock;
U32 __cdecl compileBlock_Hook(StmtNode* block, U32* codeStream, U32 ip, U32 continuePoint, U32 breakPoint) {
    //printf("compileBlock_Hook()\n");
    //unsigned int lastIp = OriginalcompileBlock(block, codeStream, ip, continuePoint, breakPoint);
    g_code = codeStream;
    return OriginalcompileBlock(block, codeStream, ip, continuePoint, breakPoint);
}

// Game
typedef signed int(__cdecl* PddiCreate_t)(int a1, int a2, int* a3);
PddiCreate_t OriginalPddiCreate;
signed int PddiCreate_Hook(int a1, int a2, int* a3) {
    printf("PddiCreate_Hook()\n");
    int res = OriginalPddiCreate(a1, a2, a3);

    //bool hooked = InstallEndScene_Hook();
    //printf("End Scene hook: %d\n", hooked);

    auto pattern1002 = hook::pattern("50 6A 01 57 51 FF 52 40").count_hint(1);
    if (pattern1002.size() == 1)
    {
        printf("[INFO]: Game version: 1.00.2\n");

        DetourFunction((PBYTE)pattern1002.get_first(0), (PBYTE)CreateD3D9DeviceHook);
        CreateD3D9DeviceRET = pattern1002.get_first(+8);
    }
    return res;
}

//00457160
typedef char(__cdecl* CreateGameWindow_t)(UINT a1);
CreateGameWindow_t OriginalCreateGameWindow;
char __cdecl CreateGameWindow_Hook(UINT a1) {
    printf("CreateGameWindow_Hook()\n");
    return OriginalCreateGameWindow(a1);
}

static HRESULT __stdcall OnCreateD3D9Device(
    IDirect3D9* pDirect3D9,
    UINT Adapter,
    D3DDEVTYPE DeviceType,
    HWND hFocusWindow,
    DWORD BehaviorFlags,
    D3DPRESENT_PARAMETERS* pPresentationParameters,
    IDirect3DDevice9** ppReturnedDeviceInterface)
{
    HRESULT hResult = pDirect3D9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    if (hResult != D3D_OK)
        return hResult;

    Direct3DDevice9Proxy* pDeviceProxy = new Direct3DDevice9Proxy(*ppReturnedDeviceInterface);
    *ppReturnedDeviceInterface = pDeviceProxy;

    printf("[INFO] Game resolution: \n\t - Width: %d\n\t - Height: %d\n", pPresentationParameters->BackBufferWidth, pPresentationParameters->BackBufferHeight);

    g_pPresentationParameters = pPresentationParameters;
    hWindow = hFocusWindow;

    /*lpPrevWndProc = (WNDPROC)GetWindowLong(hFocusWindow, GWL_WNDPROC);
    SetWindowLong(hFocusWindow, GWL_WNDPROC, (LONG)NewWndProc);*/

    // Set mouse hook
    /*hHook = SetWindowsHookEx(
        WH_MOUSE_LL,
        mouseHookProc,
        GetModuleHandle(NULL),
        NULL);*/

    return D3D_OK;
}

static void* CreateD3D9DeviceRET;
static void __declspec(naked) CreateD3D9DeviceHook()
{
    _asm push eax
    _asm push 1
    _asm push edi
    _asm push ecx
    _asm call OnCreateD3D9Device
    _asm jmp CreateD3D9DeviceRET
}

void InstallPatches() {

    uint8_t* gReleaseMode = (uint8_t*)0x007C1C54;
    //uint8_t* gFinalMode = (uint8_t*)0x007C1C55;

    int iReleaseMode = gINI->ReadSetting("ReleaseMode");
    if (iReleaseMode == 0)
        *gReleaseMode = 0;
    //*gFinalMode = 0;
}

typedef void(__stdcall* p3d_printf_t)(char* args, ...);
p3d_printf_t Originalp3d_printf;
void p3d_printf_Hook(char* args, ...) {
    printf("p3d_printf_t(%s)\n", args);
    return Originalp3d_printf(args);
}


typedef char* (__stdcall* BufferPrintf_t)(char* Format, ...);
BufferPrintf_t OriginalBufferPrintf;
char* BufferPrintf_Hook(char* Format, ...) {
    char* v1; // esi
    va_list Args; // [esp+Ch] [ebp+8h] BYREF
    int unk0 = *(DWORD*)0x821DC8;
    char byte_81DDC8[6144];

    va_start(Args, Format);
    unk0 = (unk0 + 1) % 16;
    v1 = &byte_81DDC8[1024 * unk0];
    vsprintf(v1, Format, Args);
    printf(v1, Format, Args);
    printf("\n");
    return v1;
}

void Echo(char* ayy, char* lmao) {
    printf("Echo(%s, %s)\n", ayy, lmao);
}

typedef void* (__thiscall* REGISTER_METHOD_t)(void* thisptr, int a1, int a2, int a3, int a4, int a5, char* functionname, int fnref, int a8, int a9, char a10);
REGISTER_METHOD_t Original_REGISTER_METHOD;
void* __fastcall REGISTER_METHOD_Hook(void* thisptr, void*, int a1, int a2, int a3, int a4, int a5, char* functionname, int fnref, int a8, int a9, char a10) {

    printf("REGISTER_METHOD_Hook()\n");
    if (functionname)
        printf("\tFunction name: %s\n", functionname);

    printf("\tFunction address: 0x%p\n", fnref);
    return Original_REGISTER_METHOD(thisptr, a1, a2, a3, a4, a5, functionname, fnref, a8, a9, a10);
}

typedef DWORD* (__thiscall* REGISTER_METHOD_0_t)(DWORD* thisptr, int a1, char* functionname, int fnref, int a4, int a5, int a6);
REGISTER_METHOD_0_t Original_REGISTER_METHOD_0;
DWORD* __fastcall REGISTER_METHOD_0_Hook(DWORD* thisptr, void*, int a1, char* functionname, int fnref, int a4, int a5, int a6) {

    printf("REGISTER_METHOD_0_Hook()\n");
    if (functionname)
        printf("\tFunction name: %s\n", functionname);

    printf("\tFunction address: 0x%p\n", fnref);
    return Original_REGISTER_METHOD_0(thisptr, a1, functionname, fnref, a4, a5, a6);
}

typedef DWORD* (__thiscall* REGISTER_METHOD_1_t)(DWORD* thisptr, char* classname, char* functionname, int fnref, int unk3, int unk4, int unk5);
REGISTER_METHOD_1_t Original_REGISTER_METHOD_1;
DWORD* __fastcall REGISTER_METHOD_1_Hook(DWORD* thisptr, void*, char* classname, char* functionname, int fnref, int unk3, int unk4, int unk5) {

    printf("REGISTER_METHOD_1_Hook()\n");
    if (classname)
        printf("\tClass name: %s\n", classname);
    if (functionname)
        printf("\tFunction name: %s\n", functionname);

    printf("\tFunction address: 0x%p\n", fnref);
    return Original_REGISTER_METHOD_1(thisptr, classname, functionname, fnref, unk3, unk4, unk5);
}

typedef DWORD* (__thiscall* REGISTER_METHOD_2_t)(DWORD* thisptr, int a1, char* functionname, int fnref, int a4, int a5, int a6);
REGISTER_METHOD_2_t Original_REGISTER_METHOD_2;
DWORD* __fastcall REGISTER_METHOD_2_Hook(DWORD* thisptr, void*, int a1, char* functionname, int fnref, int a4, int a5, int a6) {

    printf("REGISTER_METHOD_2_Hook()\n");
    if (functionname)
        printf("\tFunction name: %s\n", functionname);

    printf("\tFunction address: 0x%p\n", fnref);
    return Original_REGISTER_METHOD_2(thisptr, a1, functionname, fnref, a4, a5, a6);
}

typedef DWORD* (__thiscall* REGISTER_METHOD_3_t)(DWORD* thisptr, int a1, char* functionname, int fnref, int a4, int a5, int a6);
REGISTER_METHOD_3_t Original_REGISTER_METHOD_3;
DWORD* __fastcall REGISTER_METHOD_3_Hook(DWORD* thisptr, void*, int a1, char* functionname, int fnref, int a4, int a5, int a6) {

    printf("REGISTER_METHOD_3_Hook()\n");
    if (functionname)
        printf("\tFunction name: %s\n", functionname);

    printf("\tFunction address: 0x%p\n", fnref);
    return Original_REGISTER_METHOD_3(thisptr, a1, functionname, fnref, a4, a5, a6);
}

typedef DWORD* (__thiscall* REGISTER_METHOD_4_t)(DWORD* thisptr, int a1, char* functionname, int fnref, int a4, int a5, int a6);
REGISTER_METHOD_4_t Original_REGISTER_METHOD_4;
DWORD* __fastcall REGISTER_METHOD_4_Hook(DWORD* thisptr, void*, int a1, char* functionname, int fnref, int a4, int a5, int a6) {

    printf("REGISTER_METHOD_4_Hook()\n");
    if (functionname)
        printf("\tFunction name: %s\n", functionname);

    printf("\tFunction address: 0x%p\n", fnref);
    return Original_REGISTER_METHOD_4(thisptr, a1, functionname, fnref, a4, a5, a6);
}


typedef int(__stdcall* dSprintf_t)(char* Dest, int unk0, char* Format, ...);
dSprintf_t OriginaldSprintf;
int dSprintf_Hook(char* Dest, int unk0, char* Format, ...) {
    printf("dSprintf_Hook()\n");

    va_list Args; // [esp+10h] [ebp+10h] BYREF

    va_start(Args, Format);


    printf("-\t");
    vprintf(Format, Args);
    printf("\n");
    return vsprintf(Dest, Format, Args);

}

void InstallHooks() {
    OriginalCreateGameWindow = (CreateGameWindow_t)DetourFunction((PBYTE)0x00457160, (PBYTE)CreateGameWindow_Hook);
    OriginalPddiCreate = (PddiCreate_t)DetourFunction((PBYTE)0x007035B0, (PBYTE)PddiCreate_Hook);
    OriginalCodeBlock_compileExec = (compileExec_t)DetourFunction((PBYTE)0x0490390, (PBYTE)CodeBlock_compileExec_Hook);
    OriginalCodeBlock_ctor = (CodeBlock_ctor_t)DetourFunction((PBYTE)0x48CFE0, (PBYTE)CodeBlock_ctor_Hook);
    OriginalcompileBlock = (compileBlock_t)DetourFunction((PBYTE)0x48D1E0, (PBYTE)compileBlock_Hook);
    OriginaldSprintf = (dSprintf_t)DetourFunction((PBYTE)0x0048A320, (PBYTE)dSprintf_Hook);
    //Originalp3d_printf = (p3d_printf_t)DetourFunction((PBYTE)0x005E1BB0, (PBYTE)p3d_printf_Hook);
    //OriginalBufferPrintf = (BufferPrintf_t)DetourFunction((PBYTE)0x004C0D70, (PBYTE)BufferPrintf_Hook);

    /*Original_REGISTER_METHOD = (REGISTER_METHOD_t)DetourFunction((PBYTE)0x004374C0, (PBYTE)REGISTER_METHOD_Hook);
    Original_REGISTER_METHOD_0 = (REGISTER_METHOD_0_t)DetourFunction((PBYTE)0x004917B0, (PBYTE)REGISTER_METHOD_0_Hook);
    Original_REGISTER_METHOD_1 = (REGISTER_METHOD_1_t)DetourFunction((PBYTE)0x00491760, (PBYTE)REGISTER_METHOD_1_Hook);
    Original_REGISTER_METHOD_2 = (REGISTER_METHOD_2_t)DetourFunction((PBYTE)0x00491670, (PBYTE)REGISTER_METHOD_2_Hook);
















    Original_REGISTER_METHOD_3 = (REGISTER_METHOD_3_t)DetourFunction((PBYTE)0x004916C0, (PBYTE)REGISTER_METHOD_3_Hook);
    Original_REGISTER_METHOD_4 = (REGISTER_METHOD_4_t)DetourFunction((PBYTE)0x00491710, (PBYTE)REGISTER_METHOD_4_Hook);*/

}

void ScarfaceMod() {

    // Initialize ini settings.
    gINI = new CINI();
    gINI->Initialize();

    // Read ini settings.
    g_iDebugConsole = gINI->ReadSetting("DebugConsole");
    g_iLog = gINI->ReadSetting("Log");

    // Initialize debug console.
    gConsole = new CConsole();
    if (g_iDebugConsole == 1)
        gConsole->Initialize();

    printf(MOD_NAME);

    // Initialize Log.
    gLog = new CLog();
    if (g_iLog == 1)
        gLog->Initialize();


    printf("Game base address: 0x%p\n", g_hGameBase);

    //typedef HWND GetWindowHWND(void);
    //GetWindowHWND* f = (GetWindowHWND*)0x00456AF0;

    InstallPatches();

    // Install hooks.
    InstallHooks();

}

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpReserved)  // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        // Initialize once for each new process.
        // Return FALSE to fail DLL load.
        DisableThreadLibraryCalls(hinstDLL);

        g_hGameBase = GetModuleHandle(NULL);

        ScarfaceMod();
        //CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ScarfaceMod, 0, 0, 0);

        //gLog->LogMessage(MessageType::WARNING_MSG, "Warning example");
        //gLog->LogMessage(MessageType::ERROR_MSG, "Error example");
        //gLog->LogMessage(MessageType::INFO_MSG, "Info example");
        break;

    case DLL_THREAD_ATTACH:
        // Do thread-specific initialization.
        break;

    case DLL_THREAD_DETACH:
        // Do thread-specific cleanup.
        break;

    case DLL_PROCESS_DETACH:
        // Perform any necessary cleanup.
        break;
    }
    return TRUE;  // Successful DLL_PROCESS_ATTACH.
}


inline StringTableEntry U32toSTE(U32 u)
{
    return *((StringTableEntry*)&u);
}

void dumpInstructions(CodeBlock* codeBlock, U32 startIp, bool upToReturn, U32 codeSize, U32* code)
{
    U32 ip = startIp;
    //char* functionStrings = (char*)(((DWORD)codeBlock) + 0x08);
    //F64* functionFloats = (F64*)(((DWORD)codeBlock) + 0x10);

    while (ip < codeSize)
    {
        const auto opcode = code[ip++];

        switch (opcode)
        {
        case OP_FUNC_DECL:
        {
            /* StringTableEntry fnName = U32toSTE(code[ip]);
             StringTableEntry fnNamespace = U32toSTE(code[ip + 1]);
             StringTableEntry fnPackage = U32toSTE(code[ip + 2]);*/
            bool hasBody = bool(code[ip + 3]);
            U32 newIp = code[ip + 4];
            U32 argc = code[ip + 5];
            printf("%i: OP_FUNC_DECL hasbody=%i newip=%i argc=%i\n",
                ip - 1, hasBody, newIp, argc);
            /*printf("%i: OP_FUNC_DECL name=%s nspace=%s package=%s hasbody=%i newip=%i argc=%i",
                ip - 1, fnName, fnNamespace, fnPackage, hasBody, newIp, argc);*/

                // Skip args.

            ip += 6 + argc;
            break;
        }

        case OP_CREATE_OBJECT:
        {
            //StringTableEntry objParent = U32toSTE(code[ip]);
            bool isDataBlock = code[ip + 1];
            bool isInternal = code[ip + 2];
            bool isSingleton = code[ip + 3];
            U32  lineNumber = code[ip + 4];
            U32 failJump = code[ip + 5];
            printf("%i: OP_CREATE_OBJECT isDataBlock=%i isInternal=%i isSingleton=%i lineNumber=%i failJump=%i\n",
                ip - 1, isDataBlock, isInternal, isSingleton, lineNumber, failJump);
            /*printf("%i: OP_CREATE_OBJECT objParent=%s isDataBlock=%i isInternal=%i isSingleton=%i lineNumber=%i failJump=%i",
                ip - 1, objParent, isDataBlock, isInternal, isSingleton, lineNumber, failJump);*/

            ip += 6;
            break;
        }

        case OP_ADD_OBJECT:
        {
            bool placeAtRoot = code[ip++];
            printf("%i: OP_ADD_OBJECT placeAtRoot=%i\n", ip - 1, placeAtRoot);
            break;
        }

        case OP_END_OBJECT:
        {
            bool placeAtRoot = code[ip++];
            printf("%i: OP_END_OBJECT placeAtRoot=%i\n", ip - 1, placeAtRoot);
            break;
        }

        /*case OP_FINISH_OBJECT:
        {
            printf("%i: OP_FINISH_OBJECT", ip - 1);
            break;
        }*/

        case OP_JMPIFFNOT:
        {
            printf("%i: OP_JMPIFFNOT ip=%i\n", ip - 1, code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFNOT:
        {
            printf("%i: OP_JMPIFNOT ip=%i\n", ip - 1, code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFF:
        {
            printf("%i: OP_JMPIFF ip=%i\n", ip - 1, code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIF:
        {
            printf("%i: OP_JMPIF ip=%i\n", ip - 1, code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFNOT_NP:
        {
            printf("%i: OP_JMPIFNOT_NP ip=%i\n", ip - 1, code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIF_NP:
        {
            printf("%i: OP_JMPIF_NP ip=%i\n", ip - 1, code[ip]);
            ++ip;
            break;
        }

        case OP_JMP:
        {
            printf("%i: OP_JMP ip=%i\n", ip - 1, code[ip]);
            ++ip;
            break;
        }

        case OP_RETURN:
        {
            printf("%i: OP_RETURN\n", ip - 1);

            if (upToReturn)
                return;

            break;
        }

        /*case OP_RETURN_VOID:
        {
            printf("%i: OP_RETURNVOID", ip - 1);

            if (upToReturn)
                return;

            break;
        }*/

        case OP_CMPEQ:
        {
            printf("%i: OP_CMPEQ\n", ip - 1);
            break;
        }

        case OP_CMPGR:
        {
            printf("%i: OP_CMPGR\n", ip - 1);
            break;
        }

        case OP_CMPGE:
        {
            printf("%i: OP_CMPGE\n", ip - 1);
            break;
        }

        case OP_CMPLT:
        {
            printf("%i: OP_CMPLT\n", ip - 1);
            break;
        }

        case OP_CMPLE:
        {
            printf("%i: OP_CMPLE\n", ip - 1);
            break;
        }

        case OP_CMPNE:
        {
            printf("%i: OP_CMPNE\n", ip - 1);
            break;
        }

        case OP_XOR:
        {
            printf("%i: OP_XOR\n", ip - 1);
            break;
        }

        case OP_MOD:
        {
            printf("%i: OP_MOD\n", ip - 1);
            break;
        }

        case OP_BITAND:
        {
            printf("%i: OP_BITAND\n", ip - 1);
            break;
        }

        case OP_BITOR:
        {
            printf("%i: OP_BITOR\n", ip - 1);
            break;
        }

        case OP_NOT:
        {
            printf("%i: OP_NOT\n", ip - 1);
            break;
        }

        case OP_NOTF:
        {
            printf("%i: OP_NOTF\n", ip - 1);
            break;
        }

        case OP_ONESCOMPLEMENT:
        {
            printf("%i: OP_ONESCOMPLEMENT\n", ip - 1);
            break;
        }

        case OP_SHR:
        {
            printf("%i: OP_SHR\n", ip - 1);
            break;
        }

        case OP_SHL:
        {
            printf("%i: OP_SHL\n", ip - 1);
            break;
        }

        case OP_AND:
        {
            printf("%i: OP_AND\n", ip - 1);
            break;
        }

        case OP_OR:
        {
            printf("%i: OP_OR\n", ip - 1);
            break;
        }

        case OP_ADD:
        {
            printf("%i: OP_ADD\n", ip - 1);
            break;
        }

        case OP_SUB:
        {
            printf("%i: OP_SUB\n", ip - 1);
            break;
        }

        case OP_MUL:
        {
            printf("%i: OP_MUL\n", ip - 1);
            break;
        }

        case OP_DIV:
        {
            printf("%i: OP_DIV\n", ip - 1);
            break;
        }

        case OP_NEG:
        {
            printf("%i: OP_NEG\n", ip - 1);
            break;
        }

        case OP_SETCURVAR:
        {
            //StringTableEntry var = U32toSTE(code[ip]);
            printf("%i: OP_SETCURVAR\n", ip - 1);
            //printf("%i: OP_SETCURVAR var=%s", ip - 1, var);
            ip++;
            break;
        }

        case OP_SETCURVAR_CREATE:
        {
            /*StringTableEntry var = U32toSTE(code[ip]);

            printf("%i: OP_SETCURVAR_CREATE var=%s", ip - 1, var);*/

            printf("%i: OP_SETCURVAR_CREATE\n", ip - 1);
            ip++;
            break;
        }

        case OP_SETCURVAR_ARRAY:
        {
            printf("%i: OP_SETCURVAR_ARRAY\n", ip - 1);
            break;
        }

        case OP_SETCURVAR_ARRAY_CREATE:
        {
            printf("%i: OP_SETCURVAR_ARRAY_CREATE\n", ip - 1);
            break;
        }

        case OP_LOADVAR_UINT:
        {
            printf("%i: OP_LOADVAR_UINT\n", ip - 1);
            break;
        }

        case OP_LOADVAR_FLT:
        {
            printf("%i: OP_LOADVAR_FLT\n", ip - 1);
            break;
        }

        case OP_LOADVAR_STR:
        {
            printf("%i: OP_LOADVAR_STR\n", ip - 1);
            break;
        }

        case OP_SAVEVAR_UINT:
        {
            printf("%i: OP_SAVEVAR_UINT\n", ip - 1);
            break;
        }

        case OP_SAVEVAR_FLT:
        {
            printf("%i: OP_SAVEVAR_FLT\n", ip - 1);
            break;
        }

        case OP_SAVEVAR_STR:
        {
            printf("%i: OP_SAVEVAR_STR\n", ip - 1);
            break;
        }

        case OP_SETCUROBJECT:
        {
            printf("%i: OP_SETCUROBJECT\n", ip - 1);
            break;
        }

        case OP_SETCUROBJECT_NEW:
        {
            printf("%i: OP_SETCUROBJECT_NEW\n", ip - 1);
            break;
        }

        /*case OP_SETCUROBJECT_INTERNAL:
        {
            printf("%i: OP_SETCUROBJECT_INTERNAL", ip - 1);
            ++ip;
            break;
        }*/

        case OP_SETCURFIELD:
        {
            /*StringTableEntry curField = U32toSTE(code[ip]);
            printf("%i: OP_SETCURFIELD field=%s", ip - 1, curField);*/
            printf("%i: OP_SETCURFIELD\n", ip - 1);
            ++ip;
        }

        case OP_SETCURFIELD_ARRAY:
        {
            printf("%i: OP_SETCURFIELD_ARRAY\n", ip - 1);
            break;
        }

        /*case OP_SETCURFIELD_TYPE:
        {
            U32 type = code[ip];
            printf("%i: OP_SETCURFIELD_TYPE type=%i", ip - 1, type);
            ++ip;
            break;
        }*/

        case OP_LOADFIELD_UINT:
        {
            printf("%i: OP_LOADFIELD_UINT\n", ip - 1);
            break;
        }

        case OP_LOADFIELD_FLT:
        {
            printf("%i: OP_LOADFIELD_FLT\n", ip - 1);
            break;
        }

        case OP_LOADFIELD_STR:
        {
            printf("%i: OP_LOADFIELD_STR\n", ip - 1);
            break;
        }

        case OP_SAVEFIELD_UINT:
        {
            printf("%i: OP_SAVEFIELD_UINT\n", ip - 1);
            break;
        }

        case OP_SAVEFIELD_FLT:
        {
            printf("%i: OP_SAVEFIELD_FLT\n", ip - 1);
            break;
        }

        case OP_SAVEFIELD_STR:
        {
            printf("%i: OP_SAVEFIELD_STR\n", ip - 1);
            break;
        }

        case OP_STR_TO_UINT:
        {
            printf("%i: OP_STR_TO_UINT\n", ip - 1);
            break;
        }

        case OP_STR_TO_FLT:
        {
            printf("%i: OP_STR_TO_FLT\n", ip - 1);
            break;
        }

        case OP_STR_TO_NONE:
        {
            printf("%i: OP_STR_TO_NONE\n", ip - 1);
            break;
        }

        case OP_FLT_TO_UINT:
        {
            printf("%i: OP_FLT_TO_UINT\n", ip - 1);
            break;
        }

        case OP_FLT_TO_STR:
        {
            printf("%i: OP_FLT_TO_STR\n", ip - 1);
            break;
        }

        case OP_FLT_TO_NONE:
        {
            printf("%i: OP_FLT_TO_NONE\n", ip - 1);
            break;
        }

        case OP_UINT_TO_FLT:
        {
            printf("%i: OP_SAVEFIELD_STR\n", ip - 1);
            break;
        }

        case OP_UINT_TO_STR:
        {
            printf("%i: OP_UINT_TO_STR\n", ip - 1);
            break;
        }

        case OP_UINT_TO_NONE:
        {
            printf("%i: OP_UINT_TO_NONE\n", ip - 1);
            break;
        }

        case OP_LOADIMMED_UINT:
        {
            U32 val = code[ip];
            printf("%i: OP_LOADIMMED_UINT val=%i\n", ip - 1, val);
            ++ip;
            break;
        }

        case OP_LOADIMMED_FLT:
        {
            F64 val = 68;//functionFloats[code[ip]]; //im missing these glibal 
            printf("%i: OP_LOADIMMED_FLT val=%f\n", ip - 1, val);
            ++ip;
            break;
        }

        case OP_TAG_TO_STR:
        {
            const char* str = "69";//functionStrings + code[ip];
            printf("%i: OP_TAG_TO_STR str=%s\n", ip - 1, str);
            ++ip;
            break;
        }

        case OP_LOADIMMED_STR:
        {
            const char* str = "70";//functionStrings + code[ip];
            printf("%i: OP_LOADIMMED_STR str=%s\n", ip - 1, str);
            ++ip;
            break;
        }

        /*case OP_DOCBLOCK_STR:
        {
            const char* str = "71";//functionStrings + code[ip];
            printf("%i: OP_DOCBLOCK_STR str=%s", ip - 1, str);
            ++ip;
            break;
        }*/

        case OP_LOADIMMED_IDENT:
        {
            /*StringTableEntry str = U32toSTE(code[ip]);
            printf("%i: OP_LOADIMMED_IDENT str=%s", ip - 1, str);*/
            printf("%i: OP_LOADIMMED_IDENT\n", ip - 1);
            ++ip;
            break;
        }

        case OP_CALLFUNC_RESOLVE:
        {
            /*StringTableEntry fnNamespace = U32toSTE(code[ip + 1]);
            StringTableEntry fnName = U32toSTE(code[ip]);
            printf("fnNamespace=%p, fnName=%p\n", fnNamespace, fnName);*/

            U32 callType = code[ip + 2];

            /* printf("%i: OP_CALLFUNC_RESOLVE name=%s nspace=%s callType=%s", ip - 1, fnName, fnNamespace,
                 callType == FunctionCall ? "FunctionCall"
                 : callType == MethodCall ? "MethodCall" : "ParentCall");*/
            printf("%i: OP_CALLFUNC_RESOLVE callType=%s\n", ip - 1,
                callType == FunctionCall ? "FunctionCall"
                : callType == MethodCall ? "MethodCall" : "ParentCall");

            ip += 3;
            break;
        }

        case OP_CALLFUNC:
        {
            /*StringTableEntry fnNamespace = U32toSTE(code[ip + 1]);
            StringTableEntry fnName = U32toSTE(code[ip]);*/
            U32 callType = code[ip + 2];

            /* printf("%i: OP_CALLFUNC name=%s nspace=%s callType=%s", ip - 1, fnName, fnNamespace,
                 callType == FunctionCall ? "FunctionCall"
                 : callType == MethodCall ? "MethodCall" : "ParentCall");*/
            printf("%i: OP_CALLFUNC callType=%s\n", ip - 1,
                callType == FunctionCall ? "FunctionCall"
                : callType == MethodCall ? "MethodCall" : "ParentCall");

            ip += 3;
            break;
        }

        case OP_ADVANCE_STR:
        {
            printf("%i: OP_ADVANCE_STR\n", ip - 1);
            break;
        }

        case OP_ADVANCE_STR_APPENDCHAR:
        {
            char ch = code[ip];
            printf("%i: OP_ADVANCE_STR_APPENDCHAR char=%c\n", ip - 1, ch);
            ++ip;
            break;
        }

        case OP_ADVANCE_STR_COMMA:
        {
            printf("%i: OP_ADVANCE_STR_COMMA\n", ip - 1);
            break;
        }

        case OP_ADVANCE_STR_NUL:
        {
            printf("%i: OP_ADVANCE_STR_NUL\n", ip - 1);
            break;
        }

        case OP_REWIND_STR:
        {
            printf("%i: OP_REWIND_STR\n", ip - 1);
            break;
        }

        case OP_TERMINATE_REWIND_STR:
        {
            printf("%i: OP_TERMINATE_REWIND_STR\n", ip - 1);
            break;
        }

        case OP_COMPARE_STR:
        {
            printf("%i: OP_COMPARE_STR\n", ip - 1);
            break;
        }

        case OP_PUSH:
        {
            printf("%i: OP_PUSH\n", ip - 1);
            break;
        }

        case OP_PUSH_FRAME:
        {
            printf("%i: OP_PUSH_FRAME\n", ip - 1);
            break;
        }

        /*case OP_ASSERT:
        {
            const char* message = "84";//functionStrings + code[ip];
            printf("%i: OP_ASSERT message=%s", ip - 1, message);
            ++ip;
            break;
        }

        case OP_BREAK:
        {
            printf("%i: OP_BREAK", ip - 1);
            break;
        }

        case OP_ITER_BEGIN:
        {
            StringTableEntry varName = U32toSTE(code[ip]);
            U32 failIp = code[ip + 1];

            printf("%i: OP_ITER_BEGIN varName=%s failIp=%i", varName, failIp);

            ++ip;
        }

        case OP_ITER_BEGIN_STR:
        {
            StringTableEntry varName = U32toSTE(code[ip]);
            U32 failIp = code[ip + 1];

            printf("%i: OP_ITER_BEGIN varName=%s failIp=%i", varName, failIp);

            ip += 2;
        }

        case OP_ITER:
        {
            U32 breakIp = code[ip];

            printf("%i: OP_ITER breakIp=%i", breakIp);

            ++ip;
        }

        case OP_ITER_END:
        {
            printf("%i: OP_ITER_END", ip - 1);
            break;
        }*/

        default:
            printf("%x %i: !!INVALID!!\n", opcode, ip - 1);
            break;
        }
    }
}