#include "Direct3DDevice9Proxy.h"
#include "hooks.hpp"
#include "logger.hpp"
#include "config.hpp"
#include "mod.hpp"
#include "pointers.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_dx9.h>
#include <backends/imgui_impl_win32.h>

// #define _LOG_FUNC_CALLS true

// if this is true and another instance of d3d9 gets created the asi will shit itself
static bool GHasCreatedImGui = false;
static WNDPROC oWndProc = NULL;
static Direct3DDevice9Proxy* GCurrentProxy = NULL;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK hkWindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
)
{
	if(GetForegroundWindow() != g_MainWindow)
	{
		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	switch (uMsg)
	{
	case WM_KEYUP:
		if(wParam == VK_INSERT)
		{
			gangsta::g_Globals.guiOpened ^= true;

			//gangsta::g_Pointers.m_Con__evaluate(gangsta::g_Globals.guiOpened ? "FreezeGame(true);" : "FreezeGame(false);", 0, 0, 0, -1);
		}
		break;
	case WM_KILLFOCUS:
		ImGui::GetIO().MouseDrawCursor = true;
		break;
	case WM_SETFOCUS:
		ImGui::GetIO().MouseDrawCursor = false;
		break;
	case WM_DESTROY:
		exit(0);
		break;
	default:
		break;
	}

	if(gangsta::g_Globals.guiOpened)
	{
		ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);

		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	
	return ::CallWindowProcA(oWndProc, hwnd, uMsg, wParam, lParam);
}

void Direct3DDevice9Proxy::hkWindowProcHandler(HWND wnd)
{
	oWndProc = (WNDPROC)::SetWindowLongPtr((HWND)wnd, GWLP_WNDPROC, (LONG)hkWindowProc);
	gangsta::Logger::Info("Old WndProc [ {:08x} ]", (uint64_t)oWndProc);
}

Direct3DDevice9Proxy::Direct3DDevice9Proxy(IDirect3DDevice9 *pDirect3DDevice9)
{
	if(GHasCreatedImGui)
	{
		MessageBoxA(0, "D3D9 ALREADY HOOKED!", "OPEN A GITHUB ISSUE!", 0);
		ExitProcess(-1);
	}

	m_pDirect3DDevice9 = pDirect3DDevice9;
	m_hasInitializedImGui = false;
	GHasCreatedImGui = true;
	GCurrentProxy = this;
}

HRESULT Direct3DDevice9Proxy::QueryInterface(REFIID riid, void** ppvObj)
{
	return m_pDirect3DDevice9->QueryInterface(riid, ppvObj);
}

ULONG Direct3DDevice9Proxy::AddRef(void)
{
	return m_pDirect3DDevice9->AddRef();
}

ULONG Direct3DDevice9Proxy::Release(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif
	
	ULONG res = m_pDirect3DDevice9->Release();

	gangsta::g_Hooks.D3D9DeviceProxyPool.erase(m_pDirect3DDevice9);

	delete this; 

	GCurrentProxy = nullptr;

	return res;
}

HRESULT Direct3DDevice9Proxy::TestCooperativeLevel(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->TestCooperativeLevel();
}

UINT Direct3DDevice9Proxy::GetAvailableTextureMem(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetAvailableTextureMem();
}

HRESULT Direct3DDevice9Proxy::EvictManagedResources(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->EvictManagedResources();
}

HRESULT Direct3DDevice9Proxy::GetDirect3D(IDirect3D9** ppD3D9)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetDirect3D(ppD3D9);
}

HRESULT Direct3DDevice9Proxy::GetDeviceCaps(D3DCAPS9* pCaps)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetDeviceCaps(pCaps);
}

HRESULT Direct3DDevice9Proxy::GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetDisplayMode(iSwapChain, pMode);
}

HRESULT Direct3DDevice9Proxy::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetCreationParameters(pParameters);
}

HRESULT Direct3DDevice9Proxy::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
}

void Direct3DDevice9Proxy::SetCursorPosition(int X, int Y, DWORD Flags)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetCursorPosition(X, Y, Flags);
}

BOOL Direct3DDevice9Proxy::ShowCursor(BOOL bShow)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->ShowCursor(bShow);
}

HRESULT Direct3DDevice9Proxy::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
}

HRESULT Direct3DDevice9Proxy::GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetSwapChain(iSwapChain, pSwapChain);
}

UINT Direct3DDevice9Proxy::GetNumberOfSwapChains(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetNumberOfSwapChains();
}

HRESULT Direct3DDevice9Proxy::Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	pPresentationParameters->FullScreen_RefreshRateInHz = 0;
	pPresentationParameters->Windowed = true;

	ImGui_ImplDX9_InvalidateDeviceObjects();

	HRESULT res = m_pDirect3DDevice9->Reset(pPresentationParameters);

	ImGui_ImplDX9_CreateDeviceObjects();

	return res;
}

HRESULT Direct3DDevice9Proxy::Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	HRESULT res = m_pDirect3DDevice9->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return res;
}

HRESULT Direct3DDevice9Proxy::GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
}

HRESULT Direct3DDevice9Proxy::GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetRasterStatus(iSwapChain, pRasterStatus);
}

HRESULT Direct3DDevice9Proxy::SetDialogBoxMode(BOOL bEnableDialogs)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetDialogBoxMode(bEnableDialogs);
}

void Direct3DDevice9Proxy::SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetGammaRamp(iSwapChain, Flags, pRamp);
}

void Direct3DDevice9Proxy::GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetGammaRamp(iSwapChain, pRamp);
}

HRESULT Direct3DDevice9Proxy::CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
}

HRESULT Direct3DDevice9Proxy::UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->UpdateTexture(pSourceTexture, pDestinationTexture);
}

HRESULT Direct3DDevice9Proxy::GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetRenderTargetData(pRenderTarget, pDestSurface);
}

HRESULT Direct3DDevice9Proxy::GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetFrontBufferData(iSwapChain, pDestSurface);
}

HRESULT Direct3DDevice9Proxy::StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
}

HRESULT Direct3DDevice9Proxy::ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->ColorFill(pSurface, pRect, color);
}

HRESULT Direct3DDevice9Proxy::CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
}

HRESULT Direct3DDevice9Proxy::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetRenderTarget(RenderTargetIndex, pRenderTarget);
}

HRESULT Direct3DDevice9Proxy::GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget)
{

	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
}

HRESULT Direct3DDevice9Proxy::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetDepthStencilSurface(pNewZStencil);
}

HRESULT Direct3DDevice9Proxy::GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetDepthStencilSurface(ppZStencilSurface);
}

HRESULT Direct3DDevice9Proxy::BeginScene(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->BeginScene();
}

HRESULT Direct3DDevice9Proxy::EndScene(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = gangsta::g_Globals.guiOpened;

	gangsta::g_Mod.RunGui(&gangsta::g_Globals.guiOpened, g_MainWindow);

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return m_pDirect3DDevice9->EndScene();
}

HRESULT Direct3DDevice9Proxy::Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->Clear(Count, pRects, Flags, Color, Z, Stencil);
}

HRESULT Direct3DDevice9Proxy::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	if(GetForegroundWindow() != g_MainWindow)
	{
		return S_OK;
	}

	return m_pDirect3DDevice9->SetTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Proxy::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Proxy::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->MultiplyTransform(State, pMatrix);
}

HRESULT Direct3DDevice9Proxy::SetViewport(CONST D3DVIEWPORT9* pViewport)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetViewport(pViewport);
}

HRESULT Direct3DDevice9Proxy::GetViewport(D3DVIEWPORT9* pViewport)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetViewport(pViewport);
}

HRESULT Direct3DDevice9Proxy::SetMaterial(CONST D3DMATERIAL9* pMaterial)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetMaterial(pMaterial);
}

HRESULT Direct3DDevice9Proxy::GetMaterial(D3DMATERIAL9* pMaterial)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetMaterial(pMaterial);
}

HRESULT Direct3DDevice9Proxy::SetLight(DWORD Index, CONST D3DLIGHT9* pLight)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetLight(Index, pLight);
}

HRESULT Direct3DDevice9Proxy::GetLight(DWORD Index, D3DLIGHT9* pLight)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetLight(Index, pLight);
}

HRESULT Direct3DDevice9Proxy::LightEnable(DWORD Index, BOOL Enable)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->LightEnable(Index, Enable);
}

HRESULT Direct3DDevice9Proxy::GetLightEnable(DWORD Index, BOOL* pEnable)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetLightEnable(Index, pEnable);
}

HRESULT Direct3DDevice9Proxy::SetClipPlane(DWORD Index, CONST float* pPlane)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice9Proxy::GetClipPlane(DWORD Index, float* pPlane)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetClipPlane(Index, pPlane);
}

HRESULT Direct3DDevice9Proxy::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetRenderState(State, Value);
}

HRESULT Direct3DDevice9Proxy::GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetRenderState(State, pValue);
}

HRESULT Direct3DDevice9Proxy::CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateStateBlock(Type, ppSB);
}

HRESULT Direct3DDevice9Proxy::BeginStateBlock(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->BeginStateBlock();
}

HRESULT Direct3DDevice9Proxy::EndStateBlock(IDirect3DStateBlock9** ppSB)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->EndStateBlock(ppSB);
}

HRESULT Direct3DDevice9Proxy::SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice9Proxy::GetClipStatus(D3DCLIPSTATUS9* pClipStatus)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetClipStatus(pClipStatus);
}

HRESULT Direct3DDevice9Proxy::GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetTexture(Stage, ppTexture);
}

HRESULT Direct3DDevice9Proxy::SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetTexture(Stage, pTexture);
}

HRESULT Direct3DDevice9Proxy::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetTextureStageState(Stage, Type, pValue);
}

HRESULT Direct3DDevice9Proxy::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetTextureStageState(Stage, Type, Value);
}

HRESULT Direct3DDevice9Proxy::GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetSamplerState(Sampler, Type, pValue);
}

HRESULT Direct3DDevice9Proxy::SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetSamplerState(Sampler, Type, Value);
}

HRESULT Direct3DDevice9Proxy::ValidateDevice(DWORD* pNumPasses)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->ValidateDevice(pNumPasses);
}

HRESULT Direct3DDevice9Proxy::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT Direct3DDevice9Proxy::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetPaletteEntries(PaletteNumber, pEntries);
}

HRESULT Direct3DDevice9Proxy::SetCurrentTexturePalette(UINT PaletteNumber)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetCurrentTexturePalette(PaletteNumber);
}

HRESULT Direct3DDevice9Proxy::GetCurrentTexturePalette(UINT *PaletteNumber)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetCurrentTexturePalette(PaletteNumber);
}

HRESULT Direct3DDevice9Proxy::SetScissorRect(CONST RECT* pRect)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetScissorRect(pRect);
}

HRESULT Direct3DDevice9Proxy::GetScissorRect(RECT* pRect)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetScissorRect(pRect);
}

HRESULT Direct3DDevice9Proxy::SetSoftwareVertexProcessing(BOOL bSoftware)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetSoftwareVertexProcessing(bSoftware);
}

BOOL Direct3DDevice9Proxy::GetSoftwareVertexProcessing(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetSoftwareVertexProcessing();
}

HRESULT Direct3DDevice9Proxy::SetNPatchMode(float nSegments)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetNPatchMode(nSegments);
}

float Direct3DDevice9Proxy::GetNPatchMode(void)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetNPatchMode();
}

HRESULT Direct3DDevice9Proxy::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT Direct3DDevice9Proxy::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->DrawIndexedPrimitive(Type, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT Direct3DDevice9Proxy::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT Direct3DDevice9Proxy::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT Direct3DDevice9Proxy::ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
}

HRESULT Direct3DDevice9Proxy::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateVertexDeclaration(pVertexElements, ppDecl);
}

HRESULT Direct3DDevice9Proxy::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetVertexDeclaration(pDecl);
}

HRESULT Direct3DDevice9Proxy::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetVertexDeclaration(ppDecl);
}

HRESULT Direct3DDevice9Proxy::SetFVF(DWORD FVF)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetFVF(FVF);
}

HRESULT Direct3DDevice9Proxy::GetFVF(DWORD* pFVF)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetFVF(pFVF);
}

HRESULT Direct3DDevice9Proxy::CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateVertexShader(pFunction, ppShader);
}

HRESULT Direct3DDevice9Proxy::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetVertexShader(pShader);
}

HRESULT Direct3DDevice9Proxy::GetVertexShader(IDirect3DVertexShader9** ppShader)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetVertexShader(ppShader);
}

HRESULT Direct3DDevice9Proxy::SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Proxy::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Proxy::SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Proxy::GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Proxy::SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Proxy::GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Proxy::SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
}

HRESULT Direct3DDevice9Proxy::GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
}

HRESULT Direct3DDevice9Proxy::SetStreamSourceFreq(UINT StreamNumber, UINT Setting)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetStreamSourceFreq(StreamNumber, Setting);
}

HRESULT Direct3DDevice9Proxy::GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetStreamSourceFreq(StreamNumber, pSetting);
}

HRESULT Direct3DDevice9Proxy::SetIndices(IDirect3DIndexBuffer9* pIndexData)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetIndices(pIndexData);
}

HRESULT Direct3DDevice9Proxy::GetIndices(IDirect3DIndexBuffer9** ppIndexData)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetIndices(ppIndexData);
}

HRESULT Direct3DDevice9Proxy::CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreatePixelShader(pFunction, ppShader);
}

HRESULT Direct3DDevice9Proxy::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetPixelShader(pShader);
}

HRESULT Direct3DDevice9Proxy::GetPixelShader(IDirect3DPixelShader9** ppShader)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetPixelShader(ppShader);
}

HRESULT Direct3DDevice9Proxy::SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Proxy::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

HRESULT Direct3DDevice9Proxy::SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Proxy::GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

HRESULT Direct3DDevice9Proxy::SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}

HRESULT Direct3DDevice9Proxy::GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
}
HRESULT Direct3DDevice9Proxy::DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT Direct3DDevice9Proxy::DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}

HRESULT Direct3DDevice9Proxy::DeletePatch(UINT Handle)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->DeletePatch(Handle);
}

HRESULT Direct3DDevice9Proxy::CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	#if _LOG_FUNC_CALLS
	gangsta::Logger::Info("[{}]", __FUNCTION__);
	#endif

	return m_pDirect3DDevice9->CreateQuery(Type, ppQuery);
}

Direct3DDevice9Proxy* Direct3DDevice9Proxy::GetCurrentProxy()
{
	return GCurrentProxy;
}