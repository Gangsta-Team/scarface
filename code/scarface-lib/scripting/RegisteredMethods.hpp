#ifndef _REGISTEREDMETHODS
#define _REGISTEREDMETHODS

namespace con
{
    using RegisteredMethod_ReturnVoid   =   bool(__cdecl*)(void*, int arg_count, char** arg_string);
    using RegisteredMethod_ReturnBool   =   bool(__cdecl*)(void*, int arg_count, char** arg_string);
    using RegisteredMethod_ReturnFloat  =   float(__cdecl*)(void*, int arg_count, char** arg_string);
    using RegisteredMethod_ReturnInt    =   int(__cdecl*)(void*, int arg_count, char** arg_string);
    using RegisteredMethod_ReturnString =   const char*(__cdecl*)(void*, int arg_count, char** arg_string);

    class RegisteredMethod
    {
    public:
        RegisteredMethod(int _min_args, int _max_args, const char* _class_name, const char* _method_name)
        {
            func_return_string = nullptr;
            func_return_int = nullptr;
            func_return_float = nullptr;
            func_return_void = nullptr;
            func_return_bool = nullptr;

            min_arg_count = _min_args;
            max_arg_count = _max_args;

            unk = 0;

            method_name = (char*)_method_name;
            class_name = (char*)_class_name;

            flink = nullptr;
        }

        void PutInList(class RegisteredMethod** method_list)
        {
            flink = *method_list;
            (*method_list) = this;
        }
    public:
        RegisteredMethod_ReturnString func_return_string;
        RegisteredMethod_ReturnInt func_return_int;
        RegisteredMethod_ReturnFloat func_return_float;
        RegisteredMethod_ReturnVoid func_return_void;
        RegisteredMethod_ReturnBool func_return_bool;
        int min_arg_count;
        int max_arg_count;

        // just set this to null!
        int unk;
        char *method_name;
        char *class_name;
        RegisteredMethod *flink;
    };
}

#endif  _REGISTEREDMETHODS