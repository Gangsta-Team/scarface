#include "logger.hpp"
#include "config.hpp"

static gangsta::Logger* sInstance = nullptr;

gangsta::Logger* gangsta::Logger::GetInstance()
{
    if(sInstance == nullptr)
    {
        sInstance = new gangsta::Logger();
        sInstance->Initialize();
    }

    return sInstance;
}

void gangsta::Logger::Initialize()
{

    if (!AttachConsole(GetCurrentProcessId()))
    {
		AllocConsole();
    }

    SetConsoleTitleW(L"Gangsta");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);

    this->outputStream.open("CONOUT$");
    this->outputText.open(g_Config.gangstaDirectory / "log.txt");

    silentLog = false;
}

void gangsta::Logger::RawLog(const char* type, const char* format, std::va_list args)
{
    std::lock_guard lock(m_mutex);

    auto time = std::time(nullptr);
    auto tm = std::localtime(&time);

    char prefix[64] = {};
    std::snprintf(prefix, sizeof(prefix) - 1, "[%02d:%02d:%02d] [%s] ", tm->tm_hour, tm->tm_min, tm->tm_sec, type);

    std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
    char* messageBuffer = (char*) malloc(messageLength);
    
    memset(messageBuffer, 0, messageLength);
    
    std::vsnprintf(messageBuffer, messageLength, format, args);

    if(silentLog == false)
    {
        outputStream << prefix << messageBuffer << std::endl;
    }
    outputText << prefix << messageBuffer << std::endl;

    free(messageBuffer);

}