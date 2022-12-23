#include "config.hpp"
#include "logger.hpp"

namespace gangsta
{

    void CConfig::Load()
    {
        using CConfigDefaultCheck = std::function<bool(nlohmann::json& output)>;

        std::vector<CConfigDefaultCheck> cfgInitValues = {
            [] (nlohmann::json& input) -> bool
            {
                if(input["Log"].is_null())
                {
                    input["Log"] = false;
                    return true;
                }
                return false;
            },
            [] (nlohmann::json& input) -> bool
            {
                if(input["DebugConsole"].is_null())
                {
                    input["DebugConsole"] = false;
                    return true;
                }
                return false;
            },
            [] (nlohmann::json& input) -> bool
            {
                if(input["ReleaseMode"].is_null())
                {
                    input["ReleaseMode"] = false;
                    return true;
                }
                return false;
            },
            [] (nlohmann::json& input) -> bool
            {
                if(input["WindowedSpoof"].is_null())
                {
                    input["WindowedSpoof"] = false;
                    return true;
                }
                return false;
            },
            [] (nlohmann::json& input) -> bool
            {
                if(input["ImGuiTextSize"].is_null())
                {
                    // using (float) just to make sure that it doesn't do any funny
                    input["ImGuiTextSize"] = (float)16.f;
                    return true;
                }
                return false;
            },
            [] (nlohmann::json& input) -> bool
            {
                if(input["DumpCodeBlocks"].is_null())
                {
                    input["DumpCodeBlocks"] = false;
                    return true;
                }
                return false;
            },
        };

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

            std::ofstream cfgStream;
            cfgStream.open(cfgPath);

            for(auto& x : cfgInitValues)
            {
                x(parsedJson);
            }

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

        bool cfgReSave = false;

        for(auto& x : cfgInitValues)
        {
            cfgReSave |= x(parsedJson);
        }

        cfgStream.close();

        if(cfgReSave)
        {
            std::ofstream sCfgStream;
            sCfgStream.open(cfgPath);
            sCfgStream << parsedJson.dump(1);
            sCfgStream.close();
        }

    }

}