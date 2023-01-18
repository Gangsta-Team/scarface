#include "common.hpp"
#include "mod.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "config.hpp"

#include "utils/ImGuiExtras.hpp"

#include "tools/nativedumper/nativeDumper.hpp"

#include <imgui.h>
#include <radkey.hpp>

#include "d3d9/Direct3DDevice9Proxy.h"
#include "d3d9/Direct3DProxy.h"
#include "d3d9/d3dx_include.h"

#include <d3d9/d3d.h>
#include <d3d9/Colors.h>
#include <d3d9/d3dh.h>

// - stwiy-lib
#include <gameobject/character/characterobject.hpp>

CharacterObject *pMainChar = NULL;

D3DHelper* d3dh;

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

    if(ImGui::Begin("Debug"))
    {
        // Game information
        Direct3DDevice9Proxy* currentProxy = Direct3DDevice9Proxy::GetCurrentProxy();
        D3DVIEWPORT9 d3dvp;
        currentProxy->GetViewport(&d3dvp);
        
        if(d3dh == NULL)
	        d3dh = new D3DHelper(currentProxy->m_pDirect3DDevice9);

        ImGui::BeginGroup();
        
        ImGui::Separator();
        ImGui::Text("General game information:");
        ImGui::Separator();

        ImGui::Text("Viewport Width: %d", d3dvp.Width);
        ImGui::Text("Viewport Height: %d", d3dvp.Height);

        ImGui::EndGroup();

        if(pMainChar == NULL)
            pMainChar = GetMainCharacter();

        if(pMainChar){
            ImGui::BeginGroup();

            ImGui::Separator();
            ImGui::Text("Main character information (%p):", pMainChar);
            ImGui::Separator();
            
            math::Vector vecMainChar = {
                *(float*)((DWORD*)pMainChar + 0x1A),
                *(float*)((DWORD*)pMainChar + 0x19),
                *(float*)((DWORD*)pMainChar + 0x1B)
            };

            ImGui::Text("Pos X: [%.5f]", vecMainChar.x);
            ImGui::Text("Pos Y: [%.5f]", vecMainChar.y);
            ImGui::Text("Pos Z: [%.5f]", vecMainChar.z);

            math::Vector* vec_Shoulder_L = pMainChar->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Shoulder_L);

            ImGui::Text("vec_Shoulder_L Pos X: [%.5f]", vec_Shoulder_L->x);
            ImGui::Text("vec_Shoulder_L Pos Y: [%.5f]", vec_Shoulder_L->y);
            ImGui::Text("vec_Shoulder_L Pos Z: [%.5f]", vec_Shoulder_L->z);
        
            ImGui::EndGroup();

            if(CameraManager* camMgrObj = CameraManager::GetInstance())
            {
                ImGui::Separator();
                ImGui::Text("Camera manager information (%p):", camMgrObj);
                ImGui::Separator();

                pure3d::Camera* pCam = camMgrObj->GetCurrentRenderCamera();
                if(pCam != NULL)
                {
                    ImGui::Text("Camera manager pCam (%p):", pCam);
                    pure3d::VectorCamera* thisCam = reinterpret_cast<pure3d::VectorCamera*>(pCam);
                    if(thisCam != NULL){
                        math::Vector cpoint = {0,0,0};
                        ImGui::Text("thisCam: %p", thisCam);
                        thisCam->WorldToViewport(vec_Shoulder_L, &cpoint);
                        ImGui::Text("vec_Shoulder_L Screen X: [%.5f]", cpoint.x);
                        ImGui::Text("vec_Shoulder_L Screen Y: [%.5f]", cpoint.y);

                        RECT rect;
                        rect.top = 0;
                        rect.left = 0;
                        rect.right = d3dvp.Width;
                        rect.bottom = d3dvp.Height;
                        d3dh->DrawLine(rect, 1.0f, D3DCOLOR_ARGB(255, 69, 69, 69), D3DCOLOR_ARGB(255, 69, 69, 69));

                        RECT rect_bone;
                        rect_bone.top = d3dvp.Width / 2;
                        rect_bone.left = d3dvp.Height / 2;
                        rect_bone.right = cpoint.x * 5.0;
                        rect_bone.bottom = cpoint.y * 5.0;
                        ImGui::Text("Right X: [%.5f]", rect_bone.right);
                        ImGui::Text("Bottom Y: [%.5f]", rect_bone.bottom);
                        d3dh->DrawLine(rect_bone, 1.0f, D3DCOLOR_ARGB(255, 255, 0, 0), D3DCOLOR_ARGB(255, 255, 0, 0));

                    }
                }
            } 
        }
    }
    ImGui::End();

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

                        torque3d::CodeBlock** firstBlockPointer = torque3d::smCodeBlockList;
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
                                char *m_global_strings;
                                char *m_function_strings;
                                void *m_global_floats;
                                void *m_function_floats;
                                uint32_t m_code_size;
                                uint32_t *m_code;
                                uint32_t m_ref_count;
                                uint32_t m_line_break_pair_count;
                                uint32_t *m_line_break_pairs;
                                uint32_t m_break_list_size;
                                uint32_t *m_break_list;
                                */

                                static std::vector<TableEntryFunc> fEntryFuncs
                                =
                                {
                                    { "Name", TableEntryFuncRenderType::STIRNG_FILEHASH, offsetof(torque3d::CodeBlock, m_name) },
                                    { "Global Strings", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_global_strings) },
                                    { "Function Strings", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_function_strings) },
                                    { "Global Floats", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_global_floats) },
                                    { "Function Floats", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_function_floats) },
                                    { "Code Size", TableEntryFuncRenderType::UINT, offsetof(torque3d::CodeBlock, m_code_size) },
                                    { "Code", TableEntryFuncRenderType::POINTER, offsetof(torque3d::CodeBlock, m_code) },
                                    { "Ref Count", TableEntryFuncRenderType::INT, offsetof(torque3d::CodeBlock, m_ref_count) },
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

                                if(ImGui::Button("Terminate"))
                                {
                                    selectedCodeBlockPointer->m_ref_count = -1;
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
                    if(ImGui::BeginChild("##RegMethodChild", { -1, -1 }, true, ImGuiWindowFlags_MenuBar))
                    {
                        if(ImGui::BeginMenuBar())
                        {
                            if(ImGui::MenuItem("Dump Methods"))
                            {
                                ImGuiFileDialog::Instance()->OpenDialog("NativeSaveDialog", "Native Dump [ Save ]", ".txt", ".");
                            }

                            ImGui::EndMenuBar();
                        }
                        con::RegisteredMethod* curMethod = *con::smRegisteredMethods;

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
                    }
                    ImGui::EndChild();
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
                    ImGui::Separator();
                    {
                        if(TODObject* todObj = TODObject::GetInstance())
                        {
                            bool changeFlag = false;

                            changeFlag |= ImGui::InputInt("Hour", (int*)&todObj->mHours, 0, 0);
                            changeFlag |= ImGui::InputInt("Minute", (int*)&todObj->mMinutes, 0, 0);
                            changeFlag |= ImGui::InputFloat("Time Speed", &todObj->mTimeOfDaySpeed);
                            changeFlag |= ImGui::InputFloat("Rain Percentage", &todObj->mRainPercentage);
                            changeFlag |= ImGui::Checkbox("Enable Rain", &todObj->mEnableRaining);

                            if(changeFlag)
                            {
                                TODObject::GetInstance()->ApplyChanges(false);
                            }
                        }
                    }
                    ImGui::Separator();
                    if(ImGui::Button("Dump Allocators"))
                    {
                        char** allocatorList = (char**)0x007BE2C0;

                        for(int i = 0; i < 25; i++)
                        {
                            Logger::Info("- {}", allocatorList[i]);
                        }
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Dump TODObject"))
                    {
                        Logger::Info("Hour: {}", TODObject::GetInstance()->mHours);
                        Logger::Info("Enable Raining: {}", TODObject::GetInstance()->mEnableRaining);
                        Logger::Info("Funky Flag: {}", TODObject::GetInstance()->HasAName());
                        Logger::Info("Name: {}", TODObject::GetInstance()->objectName);
                        Logger::Info("Name Hash: {:08x}", TODObject::GetInstance()->objectNameHash);
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

        if(ImGuiFileDialog::Instance()->Display("NativeSaveDialog"))
        {
            if(ImGuiFileDialog::Instance()->IsOk())
            {
                std::string filenameWithPath = ImGuiFileDialog::Instance()->GetFilePathName();

                gangsta::Logger::Info("Saving Native Dump: [ {} ]", filenameWithPath);

                tools::NativeDumper::Dump(filenameWithPath);
            }

            ImGuiFileDialog::Instance()->Close();
        }
    }
}