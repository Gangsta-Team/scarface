#ifndef _STACKWALKERUTILS_HPP
#define _STACKWALKERUTILS_HPP

#include "../common.hpp"
#include "../logger.hpp"
#include <StackWalker/StackWalker.h>

namespace gangsta
{

    class StackWalkerToConsole : public StackWalker
    {
    protected:
    virtual void OnOutput(LPCSTR szText) { Logger::Info("[StackWalker] {}", szText); }
    };

}

#endif