#ifndef _REGISTEREDMETHODS
#define _REGISTEREDMETHODS

namespace con
{
    class RegisteredMethod
    {
    public:
        void *func_return_string;
        void *func_return_int;
        void *func_return_float;
        void *func_return_void;
        void *func_return_bool;
        int min_arg_count;
        int max_arg_count;
        int flag_3;
        char *method_name;
        char *class_name;
        RegisteredMethod *flink;
    };
}

#endif  _REGISTEREDMETHODS