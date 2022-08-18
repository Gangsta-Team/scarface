#ifndef _CODEBLOCK_H
#define _CODEBLOCK_H

#include "../common.hpp"

namespace torque3d
{

    class CodeBlock
    {
        char *name;
        char *globalStrings;
        char *functionStrings;
        void *globalFloats;
        void *functionFloats;
        uint32_t codeSize;
        uint32_t *code;
        uint32_t refCount;
        uint32_t lineBreakPairCount;
        uint32_t *lineBreakPairs;
        uint32_t breakListSize;
        uint32_t *breakList;
        CodeBlock *nextFile;
    };

    static_assert(sizeof(CodeBlock) == 52, "torque3d::CodeBlock has not the size of 52!");
}

#endif