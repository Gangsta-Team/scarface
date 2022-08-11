#include "logger.hpp"

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
    this->outputText.open("log.txt");
}

void gangsta::Logger::RawLog(const char* type, const char* format, std::va_list args)
{
    std::lock_guard lock(m_mutex);

    auto time = std::time(nullptr);
    auto tm = std::localtime(&time);

    char prefix[64] = {};
    std::snprintf(prefix, sizeof(prefix) - 1, "[%02d:%02d:%02d] [%s] ", tm->tm_hour, tm->tm_min, tm->tm_sec, type);

    std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
    auto messageBuffer = std::make_unique<char[]>(messageLength);
    
    std::uninitialized_fill_n(messageBuffer.get(), messageLength, '\0');
    std::vsnprintf(messageBuffer.get(), messageLength, format, args);

    outputStream << prefix << messageBuffer.get() << std::endl;
    outputText << prefix << messageBuffer.get() << std::endl;

    messageBuffer.reset();

}