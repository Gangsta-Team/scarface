#ifndef _CODEBLOCK_H
#define _CODEBLOCK_H

#include "../common.hpp"

namespace torque3d
{

    enum CompiledInstructions
    {
        OP_FUNC_DECL,
        OP_CREATE_OBJECT,
        OP_2, // CONTINUE;
        OP_3, // CONTINUE;
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
        OP_76, // CONTINUE;
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

    class CodeBlock
    {
    public:
        CodeBlock();
        // in the original Torque3D engine its a char*; But they have changed it to a hash!
        uint32_t m_name;
        char *m_global_strings;
        char *m_function_strings;
        void *m_global_floats;
        void *m_function_floats;
        uint32_t m_code_size;
        uint32_t *m_code;
        int32_t m_ref_count;
        uint32_t m_line_break_pair_count;
        uint32_t *m_line_break_pairs;
        uint32_t m_break_list_size;
        uint32_t *m_break_list;
        CodeBlock *m_next_file;


        static char* U32toSTE(uint32_t u);
        static void dumpInstructions(CodeBlock* codeBlock, uint32_t startIp, bool upToReturn);
    };

    static_assert(sizeof(CodeBlock) == 52, "torque3d::CodeBlock has not the size of 52!");

    inline CodeBlock** smCodeBlockList = (CodeBlock**)0x0081518C;
}

#endif