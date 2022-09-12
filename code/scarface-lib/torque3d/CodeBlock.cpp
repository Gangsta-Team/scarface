#include "CodeBlock.h"

using namespace torque3d;

torque3d::CodeBlock::CodeBlock()
{

    memset(this, 0, sizeof(*this));

}

char* CodeBlock::U32toSTE(uint32_t u)
{
    return *((char**)&u);
}

void CodeBlock::dumpInstructions(torque3d::CodeBlock* codeBlock, uint32_t startIp, bool upToReturn)
{
    uint32_t ip = startIp;
    while (ip < codeBlock->m_code_size)
    {
        const auto opcode = codeBlock->m_code[ip++];

        switch (opcode)
        {
        case OP_FUNC_DECL:
        {
            /* StringTableEntry fnName = U32toSTE(code[ip]);
             StringTableEntry fnNamespace = U32toSTE(code[ip + 1]);
             StringTableEntry fnPackage = U32toSTE(code[ip + 2]);*/
            bool hasBody = bool(codeBlock->m_code[ip + 3]);
            uint32_t newIp = codeBlock->m_code[ip + 4];
            uint32_t argc = codeBlock->m_code[ip + 5];
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
            bool isDataBlock = codeBlock->m_code[ip + 1];
            bool isInternal = codeBlock->m_code[ip + 2];
            bool isSingleton = codeBlock->m_code[ip + 3];
            uint32_t  lineNumber = codeBlock->m_code[ip + 4];
            uint32_t failJump = codeBlock->m_code[ip + 5];
            printf("%i: OP_CREATE_OBJECT isDataBlock=%i isInternal=%i isSingleton=%i lineNumber=%i failJump=%i\n",
                ip - 1, isDataBlock, isInternal, isSingleton, lineNumber, failJump);
            /*printf("%i: OP_CREATE_OBJECT objParent=%s isDataBlock=%i isInternal=%i isSingleton=%i lineNumber=%i failJump=%i",
                ip - 1, objParent, isDataBlock, isInternal, isSingleton, lineNumber, failJump);*/

            ip += 6;
            break;
        }

        case OP_ADD_OBJECT:
        {
            bool placeAtRoot = codeBlock->m_code[ip++];
            printf("%i: OP_ADD_OBJECT placeAtRoot=%i\n", ip - 1, placeAtRoot);
            break;
        }

        case OP_END_OBJECT:
        {
            bool placeAtRoot = codeBlock->m_code[ip++];
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
            printf("%i: OP_JMPIFFNOT ip=%i\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFNOT:
        {
            printf("%i: OP_JMPIFNOT ip=%i\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFF:
        {
            printf("%i: OP_JMPIFF ip=%i\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIF:
        {
            printf("%i: OP_JMPIF ip=%i\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFNOT_NP:
        {
            printf("%i: OP_JMPIFNOT_NP ip=%i\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIF_NP:
        {
            printf("%i: OP_JMPIF_NP ip=%i\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMP:
        {
            printf("%i: OP_JMP ip=%i\n", ip - 1, codeBlock->m_code[ip]);
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
            uint32_t val = codeBlock->m_code[ip];
            printf("%i: OP_LOADIMMED_UINT val=%i\n", ip - 1, val);
            ++ip;
            break;
        }

        case OP_LOADIMMED_FLT:
        {
            double val = 68;//functionFloats[code[ip]]; //im missing these glibal 
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

            uint32_t callType = codeBlock->m_code[ip + 2];

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
            uint32_t callType = codeBlock->m_code[ip + 2];

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
            char ch = codeBlock->m_code[ip];
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