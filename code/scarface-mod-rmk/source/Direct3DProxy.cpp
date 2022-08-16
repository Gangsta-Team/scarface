#include "Direct3DProxy.h"
#include "Direct3DDevice9Proxy.h"
#include "logger.hpp"
#include "hooks.hpp"
#include "config.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>

/*** IUnknown methods ***/
HRESULT Direct3DProxy::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pDirect3d9->QueryInterface(riid, ppvObj);
}

ULONG Direct3DProxy::AddRef(void)
{
	return m_pDirect3d9->AddRef();
}

ULONG Direct3DProxy::Release(void)
{
    ULONG res = m_pDirect3d9->Release();

    gangsta::g_Hooks.D3D9ProxyPool.erase((void*)m_pDirect3d9);

    delete this;

	return res;
}

 /*** IDirect3D9 methods ***/

HRESULT Direct3DProxy::RegisterSoftwareDevice(void* pInitializeFunction)
{
    return m_pDirect3d9->RegisterSoftwareDevice(pInitializeFunction);
}

UINT Direct3DProxy::GetAdapterCount(void)
{
    return m_pDirect3d9->GetAdapterCount();
}

HRESULT Direct3DProxy::GetAdapterIdentifier(UINT Adapter,DWORD Flags,D3DADAPTER_IDENTIFIER9* pIdentifier )
{
    return m_pDirect3d9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
}

UINT Direct3DProxy::GetAdapterModeCount(UINT Adapter,D3DFORMAT Format)
{
    return m_pDirect3d9->GetAdapterModeCount(Adapter, Format);
}

HRESULT Direct3DProxy::EnumAdapterModes(UINT Adapter,D3DFORMAT Format,UINT Mode,D3DDISPLAYMODE* pMode )
{
    return m_pDirect3d9-> EnumAdapterModes(Adapter, Format, Mode, pMode);
}

HRESULT Direct3DProxy::GetAdapterDisplayMode(UINT Adapter,D3DDISPLAYMODE* pMode )
{
    return m_pDirect3d9->GetAdapterDisplayMode(Adapter, pMode);
}

HRESULT Direct3DProxy:: CheckDeviceType (UINT Adapter,D3DDEVTYPE DevType,D3DFORMAT AdapterFormat,D3DFORMAT BackBufferFormat,BOOL bWindowed)
{
    bool _bWindowed = gangsta::g_Config.parsedJson["WindowedSpoof"].get<bool>();

    return m_pDirect3d9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, _bWindowed);
}

HRESULT Direct3DProxy::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat )
{
    return m_pDirect3d9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT Direct3DProxy::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
    bool _Windowed = gangsta::g_Config.parsedJson["WindowedSpoof"].get<bool>();
    return m_pDirect3d9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, _Windowed, MultiSampleType, pQualityLevels);
}

HRESULT Direct3DProxy::CheckDepthStencilMatch(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,D3DFORMAT RenderTargetFormat,D3DFORMAT DepthStencilFormat)
{
    return m_pDirect3d9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
}

HRESULT Direct3DProxy::CheckDeviceFormatConversion(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SourceFormat,D3DFORMAT TargetFormat )
{
    return m_pDirect3d9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
}

HRESULT Direct3DProxy::GetDeviceCaps(UINT Adapter,D3DDEVTYPE DeviceType,D3DCAPS9* pCaps)
{
    return m_pDirect3d9->GetDeviceCaps(Adapter, DeviceType, pCaps);
}

HMONITOR Direct3DProxy::GetAdapterMonitor(UINT Adapter)
{
    return m_pDirect3d9->GetAdapterMonitor(Adapter);
}

std::string GetLastErrorAsString()
{
    //Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if(errorMessageID == 0) {
        return std::string(); //No error message has been recorded
    }
    
    LPSTR messageBuffer = nullptr;

    //Ask Win32 to give us the string version of that message ID.
    //The parameters we pass in, tell Win32 to create the buffer that holds the message for us (because we don't yet know how long the message string will be).
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    
    //Copy the error message into a std::string.
    std::string message(messageBuffer, size);
    
    //Free the Win32's string's buffer.
    LocalFree(messageBuffer);
            
    return message;
}

HRESULT Direct3DProxy::CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface )
{
    HRESULT res = NULL;

    if(gangsta::g_Config.parsedJson["WindowedSpoof"].get<bool>())
    {
        pPresentationParameters->Windowed = true;
        pPresentationParameters->FullScreen_RefreshRateInHz = 0;

        res = m_pDirect3d9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
        
        SetWindowLong(hFocusWindow, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN);

        SetWindowPos(hFocusWindow, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    }
    else
    {
        res = m_pDirect3d9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    }

    if(SUCCEEDED(res))
    {

        Direct3DDevice9Proxy* prxy = new Direct3DDevice9Proxy(*ppReturnedDeviceInterface);

        gangsta::g_Hooks.D3D9DeviceProxyPool.emplace((void*)*ppReturnedDeviceInterface, prxy);

        *ppReturnedDeviceInterface = dynamic_cast<IDirect3DDevice9*>(prxy);
    }
    else
    {
        gangsta::Logger::GetInstance()->Error("CreateDevice: %s \n", GetLastErrorAsString().c_str());

        while(1);
    }

    if(g_MainWindow)
    {
        gangsta::Logger::GetInstance()->Error("g_MainWindow was not null! Please contact on GitHub thank you!");
        
        while(1);
    }

	g_MainWindow = pPresentationParameters->hDeviceWindow;

    Direct3DDevice9Proxy::hkWindowProcHandler(g_MainWindow);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplWin32_Init(pPresentationParameters->hDeviceWindow);
    ImGui_ImplDX9_Init((*ppReturnedDeviceInterface));

    return res;
}
