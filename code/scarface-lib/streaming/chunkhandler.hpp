#ifndef _CHUNKHANDLER_HPP
#define _CHUNKHANDLER_HPP

#include "../radobjects.hpp"
#include "chunkfile.hpp"

namespace content
{

    class SimpleChunkHandler : public core::IRefCount 
    {
    public:
        virtual ~SimpleChunkHandler() = default;
        
        virtual void __declspec(naked) Load()
        {
            __asm mov eax, 0
            __asm ret
        }

        virtual int GetChunkId() = 0;
        virtual int LoadObject(core::IRefCount** object, core::Key* name, tChunkFile* chunk, void*) = 0;
    };

}

#endif