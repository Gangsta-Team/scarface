#ifndef _LOGGER_HPP
#define _LOGGER_HPP

#include "common.hpp"

#define IMPL_LOGGER_FUNC(fn, pn)  inline void fn (const char* fmt, ...)\
        {\
            std::va_list vaList;\
            va_start(vaList, fmt);\
            RawLog(pn, fmt, vaList);\
            va_end(vaList);\
        }

namespace gangsta
{

    class Logger
    {
    public:
        static Logger* GetInstance();
        void Initialize();
    public:
        IMPL_LOGGER_FUNC(Info, "Info");
        IMPL_LOGGER_FUNC(Warning, "Warning");
        IMPL_LOGGER_FUNC(Error, "Error");
    
    private:
        void RawLog(const char* prefix, const char* fmt, std::va_list vaList);
    private:
        std::ofstream outputStream;
        std::ofstream outputText;
        std::mutex m_mutex;
    };

}

#endif // !_LOGGER_HPP