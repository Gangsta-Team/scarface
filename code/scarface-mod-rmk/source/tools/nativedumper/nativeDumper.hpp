#ifndef _NATIVEDUMPER_HPP
#define _NATIVEDUMPER_HPP

#if _ENABLE_NATIVES_DUMPER

#include "common.hpp"

namespace gangsta::tools
{

    class NativeDumper
    {
    public:
        static void Dump(std::filesystem::path outputPath);
    };

}

#endif

#endif // !_NATIVEDUMPER_HPP