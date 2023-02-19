#include "common.hpp"
#include "mod.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "config.hpp"

#include "utils/ImGuiExtras.hpp"

#include <imgui.h>
#include <radkey.hpp>

#include "d3d9/Direct3DDevice9Proxy.h"
#include "d3d9/Direct3DProxy.h"
#include "d3d9/d3dx_include.h"

#include "gameutils/pure3dHelper.hpp"

pure3dH                 *p3dh;
D3DVIEWPORT9            d3dvp;

void gangsta::CMod::InputWatcher(HWND hMainWindow) {
    ImGuiIO& io = ImGui::GetIO();
    POINT mPos;
    GetCursorPos(&mPos);
    ScreenToClient(hMainWindow, &mPos);
    io.MousePos.x = (float)mPos.x;
    io.MousePos.y = (float)mPos.y;
    io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    io.MouseDown[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    io.MouseDown[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}

void gangsta::CMod::RunGui(bool* pGui, HWND hMainWindow)
{
    if(ImGui::Begin("Debug"))
    {
        // Game information
        Direct3DDevice9Proxy* currentProxy = Direct3DDevice9Proxy::GetCurrentProxy();
        currentProxy->GetViewport(&d3dvp);

        ImGui::BeginGroup();
        
        ImGui::Text(" # General game information:");
        ImGui::Separator();

        ImGui::Text("Viewport Width: %d", d3dvp.Width);
        ImGui::Text("Viewport Height: %d", d3dvp.Height);

        ImGui::EndGroup();
    }
    ImGui::End();

    // pGui = is opened
    if(*pGui)
    {
        gangsta::g_Mod.InputWatcher(hMainWindow);
    }
}