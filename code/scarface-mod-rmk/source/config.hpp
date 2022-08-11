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

    inline CConfig g_Config;

}

#endif