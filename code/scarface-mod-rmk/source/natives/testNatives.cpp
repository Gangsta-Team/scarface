#include "common.hpp"
#include "gameEventPools.hpp"
#include "logger.hpp"

#include <IEvent.hpp>
#include <InitFunction.hpp>

using namespace gangsta;

static bool TestFunc(void*, int arg_count, char ** arg_text)
{
    Logger::Info("Called Test Function! Arg Count: {}", arg_count);
    return 1;
}

DEFINE_INIT_FUNCTION(TestInit, {
    event_pools::gNativeEventPools.OnDispatch([] 
    {
       con::RegisteredMethod::MakeNewSpaceInList(1, 1, nullptr, "GangstaTestFunc")->func_return_void = TestFunc;
    });
});