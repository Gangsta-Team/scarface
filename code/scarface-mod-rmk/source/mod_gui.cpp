#include "mod.hpp"

#include <imgui.h>
#include "logger.hpp"
#include "pointers.hpp"
#include "config.hpp"

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
    static TextEditor gScriptEditor = TextEditor();
    // pGui = is opened
    if(*pGui)
    {
        gangsta::g_Mod.InputWatcher(hMainWindow);

        if(ImGui::Begin("Gangster"))
        {

            if(ImGui::BeginTabBar("##MainTabBar"))
            {
                if(ImGui::BeginTabItem("Scripting"))
                {
                    if(ImGui::BeginChild("##ScriptingChild", {-1, -1}, true, ImGuiWindowFlags_MenuBar))
                    {
                        if(ImGui::BeginMenuBar())
                        {
                            if(ImGui::BeginMenu("Actions"))
                            {
                                
                                if(ImGui::MenuItem("Load Script File"))
                                {
                                    ImGuiFileDialog::Instance()->OpenDialog("ScriptOpenDialog", "Gangster / File Browser [ Open ]", ".cs", ".");
                                }
                                if(ImGui::MenuItem("Save Script File"))
                                {
                                    ImGuiFileDialog::Instance()->OpenDialog("ScriptSaveDialog", "Gangster / File Browser [ Save ]", ".cs", ".");
                                }

                                if (ImGui::MenuItem("Compile & Run"))
                                {
                                    static void* p_CBInstance = nullptr;
                                    if (p_CBInstance == nullptr) {
                                        BYTE* cb_ctor = (BYTE*)malloc(52);
                                        memset(cb_ctor, 0, 52);
                                        p_CBInstance = gangsta::g_Pointers.m_CodeBlock_ctor(cb_ctor);
                                    }

                                    gangsta::g_Pointers.m_compileExec(p_CBInstance, NULL, (char*)gScriptEditor.GetText().c_str(), NULL);

                                }

                                if (ImGui::MenuItem("Run"))
                                {
                                    gangsta::g_Pointers.m_Con__evaluate((char*)gScriptEditor.GetText().c_str(), 0, 0, 0, -1);
                                }

                                ImGui::EndMenu();
                            }
                        }
                        ImGui::EndMenuBar();

                        // -1, -1 = max size
                        gScriptEditor.Render("##ScriptTextEditor", {-1, -1}, true);
                    }
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }

                if(ImGui::BeginTabItem("CodeBlock List"))
                {
                    if(ImGui::BeginChild("##CodeBlockListContainer", {-1, -1}))
                    {
                        ImVec2 containerSize = ImGui::GetWindowSize();

                        torque3d::CodeBlock** firstBlockPointer = g_Pointers.m_smCodeBlockList;
                        torque3d::CodeBlock* currentCodeBlock = *firstBlockPointer;

                        static int selectedCodeBlockIndex = -1;

                        torque3d::CodeBlock* selectedCodeBlockPointer = nullptr;

                        if(ImGui::BeginChild("##CodeBlockListContainer##SideBar", {containerSize.x / 3, -1}, true))
                        {
                            if(currentCodeBlock)
                            {
                                int it = 0;
                                while (1)
                                {
                                    if(currentCodeBlock == nullptr)
                                    {
                                        break;
                                    }

                                    it++;
                                    
                                    char rndrBuf[256] = {0};
                                    if(currentCodeBlock->name != NULL)
                                    {
                                        if(g_Globals.fileHashRegister.count(currentCodeBlock->name) == 0)
                                        {
                                            sprintf_s(rndrBuf, "%p", currentCodeBlock);
                                        }
                                        else
                                        {
                                            std::string fName = g_Globals.fileHashRegister[currentCodeBlock->name];
                                            sprintf_s(rndrBuf, "%s", fName.c_str());
                                        }
                                    }
                                    else
                                    {
                                        sprintf_s(rndrBuf, "%p", currentCodeBlock);
                                    }

                                    if(ImGui::Selectable(rndrBuf, it == selectedCodeBlockIndex))
                                    {
                                        selectedCodeBlockIndex = it;
                                    }

                                    if(selectedCodeBlockIndex == it)
                                    {
                                        selectedCodeBlockPointer = currentCodeBlock;
                                    }

                                    currentCodeBlock = currentCodeBlock->nextFile;
                                }
                                
                            }
                        }
                        ImGui::EndChild();
                    }

                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }
        }
        ImGui::End();
        
        if (ImGuiFileDialog::Instance()->Display("ScriptOpenDialog"))
        {
            if(ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filenameWithPath = ImGuiFileDialog::Instance()->GetFilePathName();
                // std::cout doesn't work use the logger
                // std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;

                std::ifstream ifs(filenameWithPath);
                std::string content((std::istreambuf_iterator<char>(ifs)),
                    (std::istreambuf_iterator<char>()));

                // do c_str() on a string !!!! a STRING is not a char* or const char* you need to to .c_str() that returns a const char*!!!!
                gangsta::Logger::GetInstance()->Info("Opening file: [ %s ]", filenameWithPath.c_str());

                gScriptEditor.SetText(content);
            }

            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGuiFileDialog::Instance()->Display("ScriptSaveDialog"))
        {
            if(ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filenameWithPath = ImGuiFileDialog::Instance()->GetFilePathName();

                std::ofstream ofs(filenameWithPath, std::ios::trunc);

                gangsta::Logger::GetInstance()->Info("Saving file: [ %s ]", filenameWithPath.c_str());

                ofs << gScriptEditor.GetText();
                ofs.flush();
                ofs.close();
            }

            ImGuiFileDialog::Instance()->Close();
        }
    }
}