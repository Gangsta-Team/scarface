#include "logger.hpp"
#include "config.hpp"

static std::ofstream gConStream;
static std::ofstream gFileStream;

namespace gangsta
{
    void Logger::Init()
    {
        if (!AttachConsole(GetCurrentProcessId()))
        {
            AllocConsole();
        }

        SetConsoleTitleW(L"stwiy-client");
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);

        gConStream.open("CONOUT$");
        gFileStream.open(g_Config.gangstaDirectory / "stwiy-client-log.txt");
    }

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

        gConStream << msg << std::endl;
        gFileStream << msg << std::endl;

        SetConsoleTextAttribute(conHandle, oldScreenAttr);

    }
}