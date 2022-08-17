#include "pointers.hpp"

namespace gangsta
{

    // called from @CMod::InitSafe
    void CPointers::ScanSafe()
    {
        m_gReleaseMode = (uint8_t*)0x007C1C54;
        m_pddiCreate = (PddiCreate_t)0x007035B0;
        m_compileExec = (CompileExec_t)0x00490390;
    }

    // called from @CMod::Init
    void CPointers::Scan()
    {}
}