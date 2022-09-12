#include "logger.hpp"

namespace gangsta
{
    void Logger::RawLog(std::string type, logColor col, std::string format, std::format_args args)
    {		
        CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
        HANDLE conHandle = GetStdHandle(STD_OUTPUT_HANDLE);

        GetConsoleScreenBufferInfo(conHandle, &bufferInfo);

        WORD oldScreenAttr = bufferInfo.wAttributes;

        SetConsoleTextAttribute(conHandle, col);
        auto getTime = std::time(nullptr);
        auto timeStruct = std::localtime(&getTime);
        auto msg = std::format("{:02d}:{:02d}:{:02d}", timeStruct->tm_hour, timeStruct->tm_min, timeStruct->tm_sec) + " | " + type + " | " + std::vformat(format, args);

        std::cout << msg << std::endl;
        SetConsoleTextAttribute(conHandle, oldScreenAttr);

    }
}