#include "pointers.hpp"

namespace gangsta
{

    // called from @CMod::InitSafe
    void CPointers::ScanSafe()
    {
        m_gReleaseMode = (uint8_t*)0x007C1C54;
        m_pddiCreate = (PddiCreate_t)0x007035B0;

        // CodeBlock stuff
        m_CodeBlock_ctor = (CodeBlock_ctor_t)0x0048CFE0;
        m_compileExec = (CompileExec_t)0x00490390;
        m_Con__evaluate = (Con__evaluate_t)0x004922D0;
        m_smCodeBlockList = (torque3d::CodeBlock**)0x0081518C;
        m_smRegisteredMethods = (con::RegisteredMethod**)0x00816688;
    }

    // called from @CMod::Init
    void CPointers::Scan()
    {}
}