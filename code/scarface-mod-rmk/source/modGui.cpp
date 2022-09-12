#include "common.hpp"
#include "mod.hpp"
#include <imgui.h>
#include "logger.hpp"
#include "pointers.hpp"
#include "config.hpp"
#include "utils/ImGuiExtras.hpp"

#include <radkey.hpp>

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
                                    static torque3d::CodeBlock* p_CBInstance = nullptr;
                                    if (p_CBInstance == nullptr) {
                                        p_CBInstance = new torque3d::CodeBlock();
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
                                    
                                    std::string endNameBuffer = "";
                                    if(currentCodeBlock->m_name != NULL)
                                    {
                                        if(g_Globals.fileHashRegister.count(currentCodeBlock->m_name) == 0)
                                        {
                                            endNameBuffer = std::to_string((uint32_t)currentCodeBlock);
                                        }
                                        else
                                        {
                                            endNameBuffer = g_Globals.fileHashRegister[currentCodeBlock->m_name];
                                        }
                                    }
                                    else
                                    {
                                        endNameBuffer = std::to_string((uint32_t)currentCodeBlock);
                                    }

                                    if(ImGui::Selectable(endNameBuffer.c_str(), it == selectedCodeBlockIndex))
                                    {
                                        selectedCodeBlockIndex = it;
                                    }

                                    if(selectedCodeBlockIndex == it)
                                    {
                                        selectedCodeBlockPointer = currentCodeBlock;
                                    }

                                    currentCodeBlock = currentCodeBlock->m_next_file;
                                }
                                
                            }
                        }
                        ImGui::EndChild();
                        ImGui::SameLine();
                        if(ImGui::BeginChild("##CodeBlockListContainer##InfoPanel", {-1, -1}, true))
                        {
                            if(selectedCodeBlockPointer)
                            {
                                enum TableEntryFuncRenderType
                                {
                                    POINTER,
                                    STRING,
                                    STIRNG_FILEHASH,
                                    INT,
                                    UINT
                                };

                                using TableEntryFunc = struct {
                                    std::string renderName;
                                    TableEntryFuncRenderType renderType;
                                    uint32_t renderOffset;
                                };

                                /*
                                uint32_t name;
                                char *globalStrings;
                                char *functionStrings;
                                void *globalFloats;
                                void *functionFloats;
                                uint32_t codeSize;
                                uint32_t *code;
                                uint32_t refCount;
                                uint32_t lineBreakPairCount;
                                uint32_t *lineBreakPairs;
                                uint32_t breakListSize;
                                uint32_t *breakList;
                                CodeBlock *nextFile;
                                */

                                static std::vector<TableEntryFunc> fEntryFuncs
                                =
                                {
                                    { "Name", TableEntryFuncRenderType::STIRNG_FILEHASH, offsetof(torque3d::CodeBlock, m_name) },
                                    { "Global Strings", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_global_strings) },
                                    { "Function Strings", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_function_strings) },
                                    { "Global Floats", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_global_floats) }
                                };

                                

                                if(ImGui::BeginTable("##DisplayTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
                                {
                                    for(auto e : fEntryFuncs)
                                    {
                                        ImGui::TableNextRow();
                                        ImGui::TableSetColumnIndex(0);
                                        ImGui::Text("%s", e.renderName.c_str());
                                        ImGui::TableSetColumnIndex(1);
                                        uintptr_t valueBegin = (uintptr_t)selectedCodeBlockPointer;
                                        switch (e.renderType)
                                        {
                                        case TableEntryFuncRenderType::POINTER:
                                        {
                                            void* val = *(void**)(valueBegin + e.renderOffset);
                                            ImGui::Text("%p", val);
                                            break;
                                        }
                                        case TableEntryFuncRenderType::STRING:
                                        {
                                            void* val = *(char**)(valueBegin + e.renderOffset);
                                            ImGui::Text("%s", val);
                                            break;
                                        }
                                        case TableEntryFuncRenderType::STIRNG_FILEHASH:
                                        {
                                            uint32_t valHash = *(uint32_t*)(valueBegin + e.renderOffset);
                                            std::string val = g_Globals.fileHashRegister[valHash];
                                            ImGui::Text("%s", val.c_str());
                                            break;
                                        }
                                        case TableEntryFuncRenderType::INT:
                                        {
                                            int val = *(int*)(valueBegin + e.renderOffset);
                                            ImGui::Text("%i", val);
                                            break;
                                        }
                                        case TableEntryFuncRenderType::UINT:
                                        {
                                            uint32_t val = *(uint32_t*)(valueBegin + e.renderOffset);
                                            ImGui::Text("%u", val);
                                            break;
                                        }
                                        default:
                                            break;
                                        }
                                    }

                                    ImGui::EndTable();
                                }
                            }
                        }
                        ImGui::EndChild();
                    }

                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }

                if(ImGui::BeginTabItem("Registered Methods"))
                {
                    con::RegisteredMethod* curMethod = *g_Pointers.m_smRegisteredMethods;

                    if(curMethod)
                    {
                        while(curMethod != nullptr)
                        {
                            if(curMethod->method_name)
                            {
                                std::string funcName = "";

                                if(curMethod->class_name != nullptr)
                                {
                                    funcName += curMethod->class_name;
                                    funcName += "::";
                                }

                                funcName += curMethod->method_name;

                                ImGui::Text("%s", funcName.c_str());
                            }
                            curMethod = curMethod->flink;
                        }
                    }

                    ImGui::EndTabItem();
                }

                if(ImGui::BeginTabItem("Testing"))
                {

                    {
                        static char bbuf[256] = {0};
                        static bool ignoreCase = true;

                        ImGui::InputText("##testhash", bbuf, sizeof(bbuf)-1);
                        ImGui::Checkbox("Ignore Case", &ignoreCase);
                        if(ImGui::Button("Hash It"))
                        {
                            Logger::Info("Hash: {:08x}", core::MakeKey(bbuf, ignoreCase));
                        }
                    }

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
                gangsta::Logger::Info("Opening file: [ {} ]", filenameWithPath);

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

                gangsta::Logger::Info("Saving file: [ {} ]", filenameWithPath);

                ofs << gScriptEditor.GetText();
                ofs.flush();
                ofs.close();
            }

            ImGuiFileDialog::Instance()->Close();
        }
    }
}