#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include "common.hpp"

#define IMPL_LOGGER_FUNC(fn, pn, col)  template <typename ...arguments> \
        static void fn (std::string fmt, arguments... args) \
        { \
            RawLog( pn , col, fmt, std::make_format_args(args...)); \
        }

namespace gangsta
{

    enum logColor : uint16_t {
        red = FOREGROUND_RED,
        green = FOREGROUND_GREEN,
        blue = FOREGROUND_BLUE,
        intensify = FOREGROUND_INTENSITY,
        white = red | green | blue | intensify,
        grey = intensify,
        lightRed = red | intensify,
        lightGreen = green | intensify,
        lightBlue = blue | intensify,
    };

    inline logColor operator|(logColor a, logColor b) {
        return static_cast<logColor>(static_cast<std::underlying_type_t<logColor>>(a) | static_cast<std::underlying_type_t<logColor>>(b));
    }

    class Logger
    {
    public:
        IMPL_LOGGER_FUNC(Info, "Info", logColor::grey);
        IMPL_LOGGER_FUNC(Warning, "Warning", logColor::white);
        IMPL_LOGGER_FUNC(Error, "Error", logColor::lightRed);

    private:
        static void RawLog(std::string prefix, logColor col, std::string fmt, std::format_args vaList);
    };

}

#endif // !_LOGGER_HPP