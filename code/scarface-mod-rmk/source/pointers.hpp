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
    public:
        // variables like CTest* m_test;
        uint8_t *m_gReleaseMode;
        PddiCreate_t m_pddiCreate;
    };

    inline CPointers g_Pointers;

}

#endif