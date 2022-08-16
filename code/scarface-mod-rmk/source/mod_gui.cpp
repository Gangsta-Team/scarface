#include "mod.hpp"

#include <imgui.h>

void gangsta::CMod::RunGui(bool* pGui)
{
    // pGui = is opened
    if(*pGui)
    {
        if(ImGui::Begin("Test"))
        {

        }
        ImGui::End();
    }
}