#ifndef _CONFIG_HPP
#define _CONFIG_HPP

#include "common.hpp"
#include <nlohmann/json.hpp>

namespace gangsta
{

    class CConfig
    {
    public:
        nlohmann::json parsedJson;
        std::filesystem::path gangstaDirectory;

        void Load();
    };

    class CGlobals
    {
    public:
        bool guiOpened = false;
        std::map<uint32_t, std::string> fileHashRegister;
    };

    inline CConfig g_Config;
    inline CGlobals g_Globals;

}

#endif