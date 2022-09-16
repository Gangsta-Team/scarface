#include "common.hpp"
#include "gameEventPools.hpp"
#include "logger.hpp"
#include "d3d9/Direct3DDevice9Proxy.h"
#include "d3d9/Direct3DProxy.h"
#include "d3d9/d3dx_include.h"

#include <IEvent.hpp>
#include <InitFunction.hpp>

using namespace gangsta;

static const char* screenshotPathBuffer = nullptr;

const char* __cdecl GangstaScreenCapture(void*, int arg_count, char** arg_string)
{
    if(screenshotPathBuffer)
    {
        free((void*)screenshotPathBuffer);
        screenshotPathBuffer = nullptr;
    }

    auto d3dx9Handle = GetModuleHandleA("d3dx9_29.dll");

    if(d3dx9Handle == nullptr)
    {
        d3dx9Handle = LoadLibraryA("d3dx9_29.dll");

        if(d3dx9Handle == nullptr)
        {
            Logger::Error("Could not load d3dx9_29.dll!");
            return "error";
        }
    }

    std::filesystem::path absScreenPath = std::filesystem::current_path();
    absScreenPath /= arg_string[1];
    std::string absScreenPathString = absScreenPath.string();
    screenshotPathBuffer = strdup(absScreenPathString.c_str());

    Direct3DDevice9Proxy* currentProxy = Direct3DDevice9Proxy::GetCurrentProxy();
    D3DDISPLAYMODE displayMode = {};
    IDirect3DSurface9* pSurface = nullptr;

    currentProxy->GetDisplayMode(NULL, &displayMode);
    currentProxy->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSurface);

    D3DXSaveSurfaceToFile_t pD3DXSaveSurfaceToFile = (D3DXSaveSurfaceToFile_t)GetProcAddress(d3dx9Handle, "D3DXSaveSurfaceToFileA");

    pD3DXSaveSurfaceToFile(screenshotPathBuffer, D3DXIFF_BMP, pSurface, NULL, NULL);

    return screenshotPathBuffer;
}

bool __cdecl GangstaScreenRelease(void*, int arg_count, char** arg_string)
{
    if(screenshotPathBuffer)
    {
        free((void*)screenshotPathBuffer);
        screenshotPathBuffer = nullptr;
        return true;
    }

    return false;
}

DEFINE_INIT_FUNCTION(UtilNativesInit, {
    event_pools::gNativeEventPools.OnDispatch([] {
        con::RegisteredMethod::MakeNewSpaceInList(2, 2, nullptr, "GangstaScreenCapture")->func_return_string = GangstaScreenCapture;
        con::RegisteredMethod::MakeNewSpaceInList(1, 1, nullptr, "GangstaScreenRelease")->func_return_void = GangstaScreenRelease;
    });
});