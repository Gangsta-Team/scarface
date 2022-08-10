#ifndef _HOOKINGUTILS_HPP
#define _HOOKINGUTILS_HPP

#include <MinHook.h>

template<typename T>
inline T DetourFunction(void* trg, void* det)
{
    PVOID org = nullptr;

    MH_CreateHook(trg, det, &org);

    return (T)org;
}

template<typename T>
inline T DetourFunction(unsigned char* trg, unsigned char* det)
{
    return DetourFunction<T>((void*)trg, (void*)det);
}

#endif