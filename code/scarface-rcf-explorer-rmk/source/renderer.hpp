#ifndef  _RENDERER_HPP
#define _RENDERER_HPP

#include "common.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#define ENABLE_VEGA_SYSTEM true

namespace vega
{
	class Renderer
	{
	private:

	public:

		void Initialize();
		void BeginRender();
		void EndRender();
		void Shutdown();
		void ApplyTheme();
	public:
		HWND wnd;
        bool running;
		bool showMenuBar;
	};

	inline Renderer g_Renderer;

	bool CreateDeviceD3D(HWND hWnd);
	void CleanupDeviceD3D();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

#endif // ! _RENDERER_HPP