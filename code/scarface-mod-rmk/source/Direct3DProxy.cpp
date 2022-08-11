#include "Direct3DProxy.h"
#include "Direct3DDevice9Proxy.h"
#include "logger.hpp"
#include "hooks.hpp"

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
    return m_pDirect3d9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
}

HRESULT Direct3DProxy::CheckDeviceFormat(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT AdapterFormat,DWORD Usage,D3DRESOURCETYPE RType,D3DFORMAT CheckFormat )
{
    return m_pDirect3d9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
}

HRESULT Direct3DProxy::CheckDeviceMultiSampleType(UINT Adapter,D3DDEVTYPE DeviceType,D3DFORMAT SurfaceFormat,BOOL Windowed,D3DMULTISAMPLE_TYPE MultiSampleType,DWORD* pQualityLevels)
{
    return m_pDirect3d9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
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

HRESULT Direct3DProxy::CreateDevice(UINT Adapter,D3DDEVTYPE DeviceType,HWND hFocusWindow,DWORD BehaviorFlags,D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DDevice9** ppReturnedDeviceInterface )
{
    HRESULT res = m_pDirect3d9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, ppReturnedDeviceInterface);
    
    if(SUCCEEDED(res))
    {

        Direct3DDevice9Proxy* prxy = new Direct3DDevice9Proxy(*ppReturnedDeviceInterface);

        gangsta::g_Hooks.D3D9DeviceProxyPool.emplace((void*)*ppReturnedDeviceInterface, prxy);

        *ppReturnedDeviceInterface = dynamic_cast<IDirect3DDevice9*>(prxy);
    }

    return res;
}
