#ifndef _REGISTEREDMETHODS
#define _REGISTEREDMETHODS

namespace con
{
    class RegisteredMethod
    {
        void* func_return_string;
        void* func_return_int;
        void* func_return_float;
        void *func_return_void;
        void *func_return_bool;
        int flag_1;
        int flag_2;
        int flag_3;
        char *method_name;
        char *class_name;
        class RegisteredMethod *flink;
    };
}

#endif  _REGISTEREDMETHODS