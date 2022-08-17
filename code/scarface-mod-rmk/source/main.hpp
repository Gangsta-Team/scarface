#ifndef _MAIN_HPP
#define _MAIN_HPP

#include "common.hpp"

BOOL WINAPI DllMain(
    HINSTANCE hinstDLL,  // handle to DLL module
    DWORD fdwReason,     // reason for calling function
    LPVOID lpvReserved );  // reserved


inline TextEditor* g_TE;

#endif