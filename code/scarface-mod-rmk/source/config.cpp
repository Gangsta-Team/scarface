#include "config.hpp"
#include "logger.hpp"

namespace gangsta
{

    void CConfig::Load()
    {
        gangstaDirectory = std::getenv("appdata");
        gangstaDirectory /= "Gangsta";

        if(std::filesystem::exists(gangstaDirectory) == false)
        {
            std::filesystem::create_directory(gangstaDirectory);
        }

        std::filesystem::path cfgPath = gangstaDirectory;
        cfgPath /= "config.json";

        if(std::filesystem::exists(cfgPath) == false)
        {
            std::string cfgPathString = cfgPath.string();
            Logger::GetInstance()->Info("Couldn't load %s! Not Existing please follow the README thanks :---)", cfgPathString.c_str());

            std::ofstream cfgStream;
            cfgStream.open(cfgPath);

            parsedJson["Log"] = (bool)false;
            parsedJson["DebugConsole"] = (bool)false;
            parsedJson["ReleaseMode"] = (bool)false;

            cfgStream << parsedJson.dump(1);
            cfgStream.close();

            return;
        }

        std::ifstream cfgStream;
        cfgStream.open(cfgPath);

        // tiny trick because i don't want to hate myself too much
        std::stringstream cfgString;
        cfgString << cfgStream.rdbuf();

        this->parsedJson = nlohmann::json::parse(cfgString);

        cfgStream.close();

    }

}