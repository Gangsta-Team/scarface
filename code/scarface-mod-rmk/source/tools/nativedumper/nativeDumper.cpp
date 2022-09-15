#include "nativeDumper.hpp"

#include <scripting/RegisteredMethods.hpp>
#include <nlohmann/json.hpp>

namespace gangsta::tools
{

    void NativeDumper::Dump(std::filesystem::path outputPath)
    {
        // Util
        std::function<nlohmann::json(con::RegisteredMethod* method)> makeFuncJson = [] (con::RegisteredMethod* method) -> nlohmann::json
        {
            static_assert (sizeof(void*) == 4);

            nlohmann::json output = {};

            int returnType = -1;
            void* nativeHandler = nullptr;

            if(nativeHandler = method->func_return_string)
            {
                returnType = 0;
            }
            else if(nativeHandler = method->func_return_int)
            {
                returnType = 1;
            }
            else if(nativeHandler = method->func_return_float)
            {
                returnType = 2;
            }
            else if(nativeHandler = method->func_return_void)
            {
                returnType = 3;
            }
            else if(nativeHandler = method->func_return_bool)
            {
                returnType = 4;
            }

            output["return"] = returnType;
            output["methodName"] = method->method_name;
            
            if(method->class_name)
            {
                output["className"] = method->class_name;
            }

            if(method->usage)
            {
                output["usage"] = method->usage;
            }

            output["minArguments"]  = method->min_arg_count;
            output["maxArguments"]  = method->max_arg_count;
            output["address"]       = std::format("0x{:08x}", (uint32_t)nativeHandler);

            return output;
        };
        // Main Function
        std::ofstream ostream;
        nlohmann::json ojson;

        ojson["returnNames"] = nlohmann::json();
        ojson["returnNames"] = std::vector<std::string>({
            "string",   // 0
            "int",      // 1
            "float",    // 2
            "void",     // 3
            "bool"      // 4
        });
        ojson["globalFunctions"] = nlohmann::json();
        ojson["classFunctions"] = nlohmann::json();

        ostream.open(outputPath, std::ios::trunc);

        con::RegisteredMethod* curMethod = *con::smRegisteredMethods;

        if(curMethod)
        {
            while(curMethod != nullptr)
            {
                if(curMethod->method_name)
                {

                    if(curMethod->class_name != nullptr)
                    {
                        if(ojson["classFunctions"][curMethod->class_name].is_null())
                        {
                            ojson["classFunctions"][curMethod->class_name] = nlohmann::json();
                        }

                        ojson["classFunctions"][curMethod->class_name][curMethod->method_name] = makeFuncJson(curMethod);
                    }
                    else
                    {
                        ojson["globalFunctions"][curMethod->method_name] = makeFuncJson(curMethod);
                    }
                }
                curMethod = curMethod->flink;
            }
        }

        ostream << ojson.dump(4);
        ostream.flush();
        ostream.close();
    }

}