#include "mod.hpp"

#include <imgui.h>
#include "logger.hpp"


void gangsta::CMod::InputWatcher(HWND hMainWindow) {
    ImGuiIO& io = ImGui::GetIO();
    POINT mPos;
    GetCursorPos(&mPos);
    ScreenToClient(hMainWindow, &mPos);
    io.MousePos.x = mPos.x;
    io.MousePos.y = mPos.y;
    io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    io.MouseDown[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
    io.MouseDown[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
}

void gangsta::CMod::RunGui(bool* pGui, HWND hMainWindow)
{
    // pGui = is opened
    if(*pGui)
    {
        gangsta::g_Mod.InputWatcher(hMainWindow);
        

        ImVec2 a;
        a.x = 500;
        a.y = 400;
        g_TE->Render("a", a, true);

        //...do other stuff like ImGui::NewFrame();

        if (ImGui::Begin("Gangster"))
        {
            // open file dialog when user clicks this button
            if (ImGui::Button("Open file"))
                fileDialog.Open();
        }
        ImGui::End();

        fileDialog.Display();

        if (fileDialog.HasSelected())
        {
            std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;

            std::ifstream ifs(fileDialog.GetSelected().string());
            std::string content((std::istreambuf_iterator<char>(ifs)),
                (std::istreambuf_iterator<char>()));

            gangsta::Logger::GetInstance()->Info("Opening file: [ %s ]", fileDialog.GetSelected().string());

            g_TE->SetText(content);
            fileDialog.ClearSelected();
        }
    }
}