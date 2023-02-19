#include "pointers.hpp"

namespace gangsta
{

    // called from @CMod::InitSafe
    void CPointers::ScanSafe()
    {
        m_gReleaseMode = (uint8_t*)0x007C1C54;
        m_pddiCreate = (PddiCreate_t)0x007035B0;

        m_gOpeningSequenceEnabled = (uint8_t*)0x00822C02;
        m_gAutoTesterFlag = (uint8_t*)0x0080F868;
        m_gSkipFrontEnd = (uint8_t*)0x0080C288;
        m_gNISDisabled = (uint8_t*)0x007BF178;
        m_gLowViolenceSKU = (uint8_t*)0x0080F869;
        m_gDevStartupMenu = (uint8_t*)0x008234C4;                              
        m_gSkipToStartMission = (uint8_t*)0x008234B8;      
        m_gSkipToStartLocation = (uint32_t*)0x008234BC;

        // CodeBlock stuff
        m_CodeBlock_ctor = (CodeBlock_ctor_t)0x0048CFE0;
        m_compileExec = (CompileExec_t)0x00490390;
        m_Con__evaluate = (Con__evaluate_t)0x004922D0;
    }

    // called from @CMod::Init
    void CPointers::Scan()
    {}
}