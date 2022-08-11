#ifndef _HOOKS_HPP
#define _HOOKS_HPP

#include "common.hpp"

namespace gangsta
{

    class CHooks
    {
    public:
        void HookSafe();
        void Hook();
    public:
        // STATIC HOOKS HERE
        static int __cdecl pddiCreate(int versionMajor, int versionMinor, class pddiDevice** device);
    public:
        // ORIGINALS HERE
        PVOID OriginalPddiCreate;
    };

    inline CHooks g_Hooks;
}

#endif