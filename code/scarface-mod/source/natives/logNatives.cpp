#include "common.hpp"
#include "gameEventPools.hpp"
#include "logger.hpp"

#include <IEvent.hpp>
#include <InitFunction.hpp>

using namespace gangsta;

static bool NativeHook_Echo(void*, int _arg_count, char** arg_text)
{
    int arg_count = _arg_count - 1;

    Logger::Info("[Echo ({})] {}: {}", arg_count - 1, arg_text[1], arg_text[2]);

    return 1;
}

static bool NativeHook_SlowEcho(void*, int _arg_count, char** arg_text)
{
    Logger::Info("[SlowEcho] {}: {}", arg_text[1], arg_text[2]);

    return 1;
}

static bool NativeHook_WarningMsg(void*, int _arg_count, char** arg_text)
{
    int arg_count = _arg_count - 1;

    Logger::Info("[WarningMsg ({})] {}: {}", arg_count - 1, arg_text[1], arg_text[2]);

    return 1;
}

static bool NativeHook_DebugMsg(void*, int _arg_count, char** arg_text)
{
    int arg_count = _arg_count - 1;

    Logger::Info("[DebugMsg ({})] {}: {}", arg_count - 1, arg_text[1], arg_text[2]);

    return 1;
}

static bool NativeHook_AssertMsg(void*, int _arg_count, char** arg_text)
{
    int arg_count = _arg_count - 1;

    Logger::Info("[AssertMsg ({})] {}: {}", arg_count - 1, arg_text[1], arg_text[2]);

    return 1;
}

DEFINE_INIT_FUNCTION(LogNativesInit, {
    event_pools::gNativeEventPools.OnDispatch([] 
    {
        con::RegisteredMethod::FindMethodInList("Echo")->func_return_void       = NativeHook_Echo;
        con::RegisteredMethod::FindMethodInList("SlowEcho")->func_return_void   = NativeHook_SlowEcho;
        con::RegisteredMethod::FindMethodInList("WarningMsg")->func_return_void = NativeHook_WarningMsg;
        con::RegisteredMethod::FindMethodInList("DebugMsg")->func_return_void   = NativeHook_DebugMsg;
        con::RegisteredMethod::FindMethodInList("AssertMsg")->func_return_void  = NativeHook_AssertMsg;
    });
});