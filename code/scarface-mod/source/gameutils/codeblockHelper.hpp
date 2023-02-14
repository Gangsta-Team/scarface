#ifndef _CODEBLOCKHELPER_HPP
#define _CODEBLOCKHELPER_HPP

#include "../logger.hpp"
#include <torque3d/CodeBlock.h>

using namespace torque3d;
using namespace gangsta;

void dumpInstructions(CodeBlock* codeBlock, uint32_t startIp, bool upToReturn)
{
    uint32_t ip = startIp;
    while (ip < codeBlock->m_code_size)
    {
        const auto opcode = codeBlock->m_code[ip++];
        Logger::Info("opcode {}",opcode);

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
            Logger::Info("{}: OP_FUNC_DECL hasbody={} newip={} argc={}\n",
                ip - 1, hasBody, newIp, argc);
            /*Logger::Info("{}: OP_FUNC_DECL name=%s nspace=%s package=%s hasbody={} newip={} argc={}",
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
            Logger::Info("{}: OP_CREATE_OBJECT isDataBlock={} isInternal={} isSingleton={} lineNumber={} failJump={}\n",
                ip - 1, isDataBlock, isInternal, isSingleton, lineNumber, failJump);
            /*Logger::Info("{}: OP_CREATE_OBJECT objParent=%s isDataBlock={} isInternal={} isSingleton={} lineNumber={} failJump={}",
                ip - 1, objParent, isDataBlock, isInternal, isSingleton, lineNumber, failJump);*/

            ip += 6;
            break;
        }

        case OP_ADD_OBJECT:
        {
            bool placeAtRoot = codeBlock->m_code[ip++];
            Logger::Info("{}: OP_ADD_OBJECT placeAtRoot={}\n", ip - 1, placeAtRoot);
            break;
        }

        case OP_END_OBJECT:
        {
            bool placeAtRoot = codeBlock->m_code[ip++];
            Logger::Info("{}: OP_END_OBJECT placeAtRoot={}\n", ip - 1, placeAtRoot);
            break;
        }

        /*case OP_FINISH_OBJECT:
        {
            Logger::Info("{}: OP_FINISH_OBJECT", ip - 1);
            break;
        }*/

        case OP_JMPIFFNOT:
        {
            Logger::Info("{}: OP_JMPIFFNOT ip={}\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFNOT:
        {
            Logger::Info("{}: OP_JMPIFNOT ip={}\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFF:
        {
            Logger::Info("{}: OP_JMPIFF ip={}\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIF:
        {
            Logger::Info("{}: OP_JMPIF ip={}\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIFNOT_NP:
        {
            Logger::Info("{}: OP_JMPIFNOT_NP ip={}\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMPIF_NP:
        {
            Logger::Info("{}: OP_JMPIF_NP ip={}\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_JMP:
        {
            Logger::Info("{}: OP_JMP ip={}\n", ip - 1, codeBlock->m_code[ip]);
            ++ip;
            break;
        }

        case OP_RETURN:
        {
            Logger::Info("{}: OP_RETURN\n", ip - 1);

            if (upToReturn)
                return;

            break;
        }

        /*case OP_RETURN_VOID:
        {
            Logger::Info("{}: OP_RETURNVOID", ip - 1);

            if (upToReturn)
                return;

            break;
        }*/

        case OP_CMPEQ:
        {
            Logger::Info("{}: OP_CMPEQ\n", ip - 1);
            break;
        }

        case OP_CMPGR:
        {
            Logger::Info("{}: OP_CMPGR\n", ip - 1);
            break;
        }

        case OP_CMPGE:
        {
            Logger::Info("{}: OP_CMPGE\n", ip - 1);
            break;
        }

        case OP_CMPLT:
        {
            Logger::Info("{}: OP_CMPLT\n", ip - 1);
            break;
        }

        case OP_CMPLE:
        {
            Logger::Info("{}: OP_CMPLE\n", ip - 1);
            break;
        }

        case OP_CMPNE:
        {
            Logger::Info("{}: OP_CMPNE\n", ip - 1);
            break;
        }

        case OP_XOR:
        {
            Logger::Info("{}: OP_XOR\n", ip - 1);
            break;
        }

        case OP_MOD:
        {
            Logger::Info("{}: OP_MOD\n", ip - 1);
            break;
        }

        case OP_BITAND:
        {
            Logger::Info("{}: OP_BITAND\n", ip - 1);
            break;
        }

        case OP_BITOR:
        {
            Logger::Info("{}: OP_BITOR\n", ip - 1);
            break;
        }

        case OP_NOT:
        {
            Logger::Info("{}: OP_NOT\n", ip - 1);
            break;
        }

        case OP_NOTF:
        {
            Logger::Info("{}: OP_NOTF\n", ip - 1);
            break;
        }

        case OP_ONESCOMPLEMENT:
        {
            Logger::Info("{}: OP_ONESCOMPLEMENT\n", ip - 1);
            break;
        }

        case OP_SHR:
        {
            Logger::Info("{}: OP_SHR\n", ip - 1);
            break;
        }

        case OP_SHL:
        {
            Logger::Info("{}: OP_SHL\n", ip - 1);
            break;
        }

        case OP_AND:
        {
            Logger::Info("{}: OP_AND\n", ip - 1);
            break;
        }

        case OP_OR:
        {
            Logger::Info("{}: OP_OR\n", ip - 1);
            break;
        }

        case OP_ADD:
        {
            Logger::Info("{}: OP_ADD\n", ip - 1);
            break;
        }

        case OP_SUB:
        {
            Logger::Info("{}: OP_SUB\n", ip - 1);
            break;
        }

        case OP_MUL:
        {
            Logger::Info("{}: OP_MUL\n", ip - 1);
            break;
        }

        case OP_DIV:
        {
            Logger::Info("{}: OP_DIV\n", ip - 1);
            break;
        }

        case OP_NEG:
        {
            Logger::Info("{}: OP_NEG\n", ip - 1);
            break;
        }

        case OP_SETCURVAR:
        {
            //StringTableEntry var = U32toSTE(code[ip]);
            Logger::Info("{}: OP_SETCURVAR\n", ip - 1);
            //Logger::Info("{}: OP_SETCURVAR var=%s", ip - 1, var);
            ip++;
            break;
        }

        case OP_SETCURVAR_CREATE:
        {
            /*StringTableEntry var = U32toSTE(code[ip]);

            Logger::Info("{}: OP_SETCURVAR_CREATE var=%s", ip - 1, var);*/

            Logger::Info("{}: OP_SETCURVAR_CREATE\n", ip - 1);
            ip++;
            break;
        }

        case OP_SETCURVAR_ARRAY:
        {
            Logger::Info("{}: OP_SETCURVAR_ARRAY\n", ip - 1);
            break;
        }

        case OP_SETCURVAR_ARRAY_CREATE:
        {
            Logger::Info("{}: OP_SETCURVAR_ARRAY_CREATE\n", ip - 1);
            break;
        }

        case OP_LOADVAR_UINT:
        {
            Logger::Info("{}: OP_LOADVAR_UINT\n", ip - 1);
            break;
        }

        case OP_LOADVAR_FLT:
        {
            Logger::Info("{}: OP_LOADVAR_FLT\n", ip - 1);
            break;
        }

        case OP_LOADVAR_STR:
        {
            Logger::Info("{}: OP_LOADVAR_STR\n", ip - 1);
            break;
        }

        case OP_SAVEVAR_UINT:
        {
            Logger::Info("{}: OP_SAVEVAR_UINT\n", ip - 1);
            break;
        }

        case OP_SAVEVAR_FLT:
        {
            Logger::Info("{}: OP_SAVEVAR_FLT\n", ip - 1);
            break;
        }

        case OP_SAVEVAR_STR:
        {
            Logger::Info("{}: OP_SAVEVAR_STR\n", ip - 1);
            break;
        }

        case OP_SETCUROBJECT:
        {
            Logger::Info("{}: OP_SETCUROBJECT\n", ip - 1);
            break;
        }

        case OP_SETCUROBJECT_NEW:
        {
            Logger::Info("{}: OP_SETCUROBJECT_NEW\n", ip - 1);
            break;
        }

        /*case OP_SETCUROBJECT_INTERNAL:
        {
            Logger::Info("{}: OP_SETCUROBJECT_INTERNAL", ip - 1);
            ++ip;
            break;
        }*/

        case OP_SETCURFIELD:
        {
            /*StringTableEntry curField = U32toSTE(code[ip]);
            Logger::Info("{}: OP_SETCURFIELD field=%s", ip - 1, curField);*/
            Logger::Info("{}: OP_SETCURFIELD\n", ip - 1);
            ++ip;
        }

        case OP_SETCURFIELD_ARRAY:
        {
            Logger::Info("{}: OP_SETCURFIELD_ARRAY\n", ip - 1);
            break;
        }

        /*case OP_SETCURFIELD_TYPE:
        {
            U32 type = code[ip];
            Logger::Info("{}: OP_SETCURFIELD_TYPE type={}", ip - 1, type);
            ++ip;
            break;
        }*/

        case OP_LOADFIELD_UINT:
        {
            Logger::Info("{}: OP_LOADFIELD_UINT\n", ip - 1);
            break;
        }

        case OP_LOADFIELD_FLT:
        {
            Logger::Info("{}: OP_LOADFIELD_FLT\n", ip - 1);
            break;
        }

        case OP_LOADFIELD_STR:
        {
            Logger::Info("{}: OP_LOADFIELD_STR\n", ip - 1);
            break;
        }

        case OP_SAVEFIELD_UINT:
        {
            Logger::Info("{}: OP_SAVEFIELD_UINT\n", ip - 1);
            break;
        }

        case OP_SAVEFIELD_FLT:
        {
            Logger::Info("{}: OP_SAVEFIELD_FLT\n", ip - 1);
            break;
        }

        case OP_SAVEFIELD_STR:
        {
            Logger::Info("{}: OP_SAVEFIELD_STR\n", ip - 1);
            break;
        }

        case OP_STR_TO_UINT:
        {
            Logger::Info("{}: OP_STR_TO_UINT\n", ip - 1);
            break;
        }

        case OP_STR_TO_FLT:
        {
            Logger::Info("{}: OP_STR_TO_FLT\n", ip - 1);
            break;
        }

        case OP_STR_TO_NONE:
        {
            Logger::Info("{}: OP_STR_TO_NONE\n", ip - 1);
            break;
        }

        case OP_FLT_TO_UINT:
        {
            Logger::Info("{}: OP_FLT_TO_UINT\n", ip - 1);
            break;
        }

        case OP_FLT_TO_STR:
        {
            Logger::Info("{}: OP_FLT_TO_STR\n", ip - 1);
            break;
        }

        case OP_FLT_TO_NONE:
        {
            Logger::Info("{}: OP_FLT_TO_NONE\n", ip - 1);
            break;
        }

        case OP_UINT_TO_FLT:
        {
            Logger::Info("{}: OP_SAVEFIELD_STR\n", ip - 1);
            break;
        }

        case OP_UINT_TO_STR:
        {
            Logger::Info("{}: OP_UINT_TO_STR\n", ip - 1);
            break;
        }

        case OP_UINT_TO_NONE:
        {
            Logger::Info("{}: OP_UINT_TO_NONE\n", ip - 1);
            break;
        }

        case OP_LOADIMMED_UINT:
        {
            uint32_t val = codeBlock->m_code[ip];
            Logger::Info("{}: OP_LOADIMMED_UINT val={}\n", ip - 1, val);
            ++ip;
            break;
        }

        case OP_LOADIMMED_FLT:
        {
            double val = 68;//functionFloats[code[ip]]; //im missing these glibal 
            Logger::Info("{}: OP_LOADIMMED_FLT val=%f\n", ip - 1, val);
            ++ip;
            break;
        }

        case OP_TAG_TO_STR:
        {
            const char* str = "69";//functionStrings + code[ip];
            Logger::Info("{}: OP_TAG_TO_STR str=%s\n", ip - 1, str);
            ++ip;
            break;
        }

        case OP_LOADIMMED_STR:
        {
            const char* str = "70";//functionStrings + code[ip];
            Logger::Info("{}: OP_LOADIMMED_STR str=%s\n", ip - 1, str);
            ++ip;
            break;
        }

        /*case OP_DOCBLOCK_STR:
        {
            const char* str = "71";//functionStrings + code[ip];
            Logger::Info("{}: OP_DOCBLOCK_STR str=%s", ip - 1, str);
            ++ip;
            break;
        }*/

        case OP_LOADIMMED_IDENT:
        {
            /*StringTableEntry str = U32toSTE(code[ip]);
            Logger::Info("{}: OP_LOADIMMED_IDENT str=%s", ip - 1, str);*/
            Logger::Info("{}: OP_LOADIMMED_IDENT\n", ip - 1);
            ++ip;
            break;
        }

        case OP_CALLFUNC_RESOLVE:
        {
            /*StringTableEntry fnNamespace = U32toSTE(code[ip + 1]);
            StringTableEntry fnName = U32toSTE(code[ip]);
            Logger::Info("fnNamespace=%p, fnName=%p\n", fnNamespace, fnName);*/

            uint32_t callType = codeBlock->m_code[ip + 2];

            /* Logger::Info("{}: OP_CALLFUNC_RESOLVE name=%s nspace=%s callType=%s", ip - 1, fnName, fnNamespace,
                 callType == FunctionCall ? "FunctionCall"
                 : callType == MethodCall ? "MethodCall" : "ParentCall");*/
            Logger::Info("{}: OP_CALLFUNC_RESOLVE callType=%s\n", ip - 1,
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

            /* Logger::Info("{}: OP_CALLFUNC name=%s nspace=%s callType=%s", ip - 1, fnName, fnNamespace,
                 callType == FunctionCall ? "FunctionCall"
                 : callType == MethodCall ? "MethodCall" : "ParentCall");*/
            Logger::Info("{}: OP_CALLFUNC callType=%s\n", ip - 1,
                callType == FunctionCall ? "FunctionCall"
                : callType == MethodCall ? "MethodCall" : "ParentCall");

            ip += 3;
            break;
        }

        case OP_ADVANCE_STR:
        {
            Logger::Info("{}: OP_ADVANCE_STR\n", ip - 1);
            break;
        }

        case OP_ADVANCE_STR_APPENDCHAR:
        {
            char ch = codeBlock->m_code[ip];
            Logger::Info("{}: OP_ADVANCE_STR_APPENDCHAR char=%c\n", ip - 1, ch);
            ++ip;
            break;
        }

        case OP_ADVANCE_STR_COMMA:
        {
            Logger::Info("{}: OP_ADVANCE_STR_COMMA\n", ip - 1);
            break;
        }

        case OP_ADVANCE_STR_NUL:
        {
            Logger::Info("{}: OP_ADVANCE_STR_NUL\n", ip - 1);
            break;
        }

        case OP_REWIND_STR:
        {
            Logger::Info("{}: OP_REWIND_STR\n", ip - 1);
            break;
        }

        case OP_TERMINATE_REWIND_STR:
        {
            Logger::Info("{}: OP_TERMINATE_REWIND_STR\n", ip - 1);
            break;
        }

        case OP_COMPARE_STR:
        {
            Logger::Info("{}: OP_COMPARE_STR\n", ip - 1);
            break;
        }

        case OP_PUSH:
        {
            Logger::Info("{}: OP_PUSH\n", ip - 1);
            break;
        }

        case OP_PUSH_FRAME:
        {
            Logger::Info("{}: OP_PUSH_FRAME\n", ip - 1);
            break;
        }

        /*case OP_ASSERT:
        {
            const char* message = "84";//functionStrings + code[ip];
            Logger::Info("{}: OP_ASSERT message=%s", ip - 1, message);
            ++ip;
            break;
        }

        case OP_BREAK:
        {
            Logger::Info("{}: OP_BREAK", ip - 1);
            break;
        }

        case OP_ITER_BEGIN:
        {
            StringTableEntry varName = U32toSTE(code[ip]);
            U32 failIp = code[ip + 1];

            Logger::Info("{}: OP_ITER_BEGIN varName=%s failIp={}", varName, failIp);

            ++ip;
        }

        case OP_ITER_BEGIN_STR:
        {
            StringTableEntry varName = U32toSTE(code[ip]);
            U32 failIp = code[ip + 1];

            Logger::Info("{}: OP_ITER_BEGIN varName=%s failIp={}", varName, failIp);

            ip += 2;
        }

        case OP_ITER:
        {
            U32 breakIp = code[ip];

            Logger::Info("{}: OP_ITER breakIp={}", breakIp);

            ++ip;
        }

        case OP_ITER_END:
        {
            Logger::Info("{}: OP_ITER_END", ip - 1);
            break;
        }*/

        default:
            Logger::Info("%x {}: !!INVALID!!\n", opcode, ip - 1);
            break;
        }
    }
}

#endif // !_CODEBLOCKHELPER_HPP
