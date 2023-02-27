#ifndef _POINTERS_HPP
#define _POINTERS_HPP

#include "common.hpp"

namespace gangsta
{

    class CPointers
    {
    public:
        void ScanSafe();
        void Scan();
    public:
        // type definitions like using or typedef
        // like struct CTest { int test; };
        using PddiCreate_t = int(*)(int versionMajor, int versionMinor, class pddiDevice** device);

        // CodeBlock
        using CodeBlock_ctor_t = void* (__thiscall*)(void* _this);
        using CompileExec_t = void* (__thiscall*)(void* _this, char* str, char* source, char* arg);
        using Con__evaluate_t = char* (__cdecl*)(const char* unk0, char unk1, char* Str, int unk3, int unk4);
        using CodeBlock_GetName_t = uint32_t (__cdecl*)(const char *Str);
    public:
        // variables like CTest* m_test;
        uint8_t *m_gReleaseMode;
        uint8_t *m_gOpeningSequenceEnabled;
        uint8_t *m_gAutoTesterFlag;
        uint8_t *m_gSkipFrontEnd;
        uint8_t *m_gNISDisabled;
        uint8_t *m_gLowViolenceSKU;
        uint8_t *m_gDevStartupMenu;                                 
        uint8_t *m_gSkipToStartMission;

        uint32_t *m_gSkipToStartLocation;

        PddiCreate_t m_pddiCreate;
        // CodeBlock
        CodeBlock_ctor_t m_CodeBlock_ctor;
        CompileExec_t m_compileExec;
        Con__evaluate_t m_Con__evaluate;
    };

    inline CPointers g_Pointers;

}

#endif