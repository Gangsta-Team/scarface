#include "main_window.hpp"
#include "renderer.hpp"

#include <imgui.h>
#include <imgui_internal.h>

static bool canNotFocusMain = false;

void RenderMainWindow()
{
    ImVec2 wndSize = ImGui::GetIO().DisplaySize;

    ImGui::SetNextWindowSize(wndSize);
    ImGui::SetNextWindowPos({ 0, 0 });
    if(ImGui::Begin("##main_view", 0, (canNotFocusMain == false ? 0:ImGuiWindowFlags_NoBringToFrontOnFocus) | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | (vega::g_Renderer.showMenuBar == true ? ImGuiWindowFlags_MenuBar : 0)))
    {
    }
    ImGui::End();
}