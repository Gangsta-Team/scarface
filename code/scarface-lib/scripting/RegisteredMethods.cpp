#include "RegisteredMethods.hpp"

#include <Windows.h>

namespace con
{

    void RegisteredMethod::PutInList()
    {
        RegisteredMethod::PutInList(con::smRegisteredMethods);
    }

    RegisteredMethod* RegisteredMethod::FindMethodInList(const char* name)
    {
        return RegisteredMethod::FindMethodInList(name, con::smRegisteredMethods);
    }

    RegisteredMethod* RegisteredMethod::FindMethodInList(const char* name, RegisteredMethod** method_list)
    {
        RegisteredMethod* currentMethod = (*method_list);

        do
        {
            currentMethod = currentMethod->flink;

            if(strcmp(currentMethod->method_name, name) == 0)
            {
                return currentMethod;
            }
        } while (currentMethod->flink != nullptr);
        
        return nullptr;
    }

    RegisteredMethod* RegisteredMethod::MakeNewSpaceInList(int _min_args, int _max_args, const char* _class_name, const char* _method_name)
    {
        return RegisteredMethod::MakeNewSpaceInList(_min_args, _max_args, _class_name, _method_name, con::smRegisteredMethods);
    }

    RegisteredMethod* RegisteredMethod::MakeNewSpaceInList(int _min_args, int _max_args, const char* _class_name, const char* _method_name, class RegisteredMethod** method_list)
    {
        con::RegisteredMethod* newMethod = new con::RegisteredMethod(_min_args, _max_args, _class_name, _method_name);
        newMethod->PutInList(method_list);

        return newMethod;
    }
}