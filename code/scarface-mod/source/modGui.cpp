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

#include "gameutils/pure3dHelper.hpp"


CharacterObject         *pMainChar = nullptr;
CVManager               *pCVManager = nullptr;
CameraManager           *pCamMgr = nullptr;
CVManager               *CVMgrObj = nullptr;
pure3d::Camera          *pCam = nullptr;
pure3d::VectorCamera    *thisCam = nullptr;
SpawnMixer              *pSpawnMixerObj = nullptr;
CVLoadObject            *pLoadObject = nullptr;
DataBroker              *pDataBrokerObject = nullptr;
pure3dH                 *p3dh;
D3DVIEWPORT9            d3dvp;

void gangsta::CMod::RenderSkeleton(CharacterObject *character)
{
    math::Vector vCharRootScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Character_Root));
    math::Vector vLeftPelvisScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Pelvis_L));
    math::Vector vLeftKneeScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Knee_L));
    math::Vector vLeftFootScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Foot_L));
    math::Vector vPelvisRScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Pelvis_R));
    math::Vector vRightKneeScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Knee_R));
    math::Vector vRightFootScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Foot_R));
    math::Vector vSpine1Screen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Spine_1));
    math::Vector vSpine2Screen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Spine_2));
    math::Vector vNeckScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Neck));
    math::Vector vHeadBoneScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Head));
    math::Vector vLeftClavicleScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Clavicle_L));
    math::Vector vLeftShoulderScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Shoulder_L));
    math::Vector vLeftElbowScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Elbow_L));
    math::Vector vLeftForarmScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Forarm_L));
    math::Vector vLeftWristScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Wrist_L));
    math::Vector vCharRoot1Screen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Character_Root1));
    math::Vector vRightClavicleScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Clavicle_R));
    math::Vector vRightShoulderScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Shoulder_R));
    math::Vector vRightElbowScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Elbow_R));
    math::Vector vRightForarmScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Forarm_R));
    math::Vector vRightWristScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Wrist_R));
    math::Vector vRightHandScreen = p3dh->WorldToScreen(thisCam, d3dvp, character->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Hand_R));

    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vCharRootScreen.x), (vCharRootScreen.y)), ImVec2(vLeftPelvisScreen.x, vLeftPelvisScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vLeftPelvisScreen.x), (vLeftPelvisScreen.y)), ImVec2(vLeftKneeScreen.x, vLeftKneeScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vLeftKneeScreen.x), (vLeftKneeScreen.y)), ImVec2(vLeftFootScreen.x, vLeftFootScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vCharRootScreen.x), (vCharRootScreen.y)), ImVec2(vPelvisRScreen.x, vPelvisRScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vPelvisRScreen.x), (vPelvisRScreen.y)), ImVec2(vRightKneeScreen.x, vRightKneeScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vRightKneeScreen.x), (vRightKneeScreen.y)), ImVec2(vRightFootScreen.x, vRightFootScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vCharRootScreen.x), (vCharRootScreen.y)), ImVec2(vSpine1Screen.x, vSpine1Screen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vSpine1Screen.x), (vSpine1Screen.y)), ImVec2(vSpine2Screen.x, vSpine2Screen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vSpine2Screen.x), (vSpine2Screen.y)), ImVec2(vNeckScreen.x, vNeckScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vNeckScreen.x), (vNeckScreen.y)), ImVec2(vHeadBoneScreen.x, vHeadBoneScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vLeftClavicleScreen.x), (vLeftClavicleScreen.y)), ImVec2(vLeftShoulderScreen.x, vLeftShoulderScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vLeftShoulderScreen.x), (vLeftShoulderScreen.y)), ImVec2(vLeftElbowScreen.x, vLeftElbowScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vLeftElbowScreen.x), (vLeftElbowScreen.y)), ImVec2(vLeftForarmScreen.x, vLeftForarmScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vLeftForarmScreen.x), (vLeftForarmScreen.y)), ImVec2(vLeftWristScreen.x, vLeftWristScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vLeftClavicleScreen.x), (vLeftClavicleScreen.y)), ImVec2(vRightClavicleScreen.x, vRightClavicleScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vRightClavicleScreen.x), (vRightClavicleScreen.y)), ImVec2(vRightShoulderScreen.x, vRightShoulderScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vRightShoulderScreen.x), (vRightShoulderScreen.y)), ImVec2(vRightElbowScreen.x, vRightElbowScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vRightElbowScreen.x), (vRightElbowScreen.y)), ImVec2(vRightForarmScreen.x, vRightForarmScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vRightForarmScreen.x), (vRightForarmScreen.y)), ImVec2(vRightWristScreen.x, vRightWristScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
    ImGui::GetBackgroundDrawList()->AddLine(ImVec2((vRightWristScreen.x), (vRightWristScreen.y)), ImVec2(vRightHandScreen.x, vRightHandScreen.y), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 255.0, 255.0, 255.0)), 1);
}

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
        currentProxy->GetViewport(&d3dvp);

        ImGui::BeginGroup();
        
        ImGui::Text(" # General game information:");
        ImGui::Separator();

        ImGui::Text("Viewport Width: %d", d3dvp.Width);
        ImGui::Text("Viewport Height: %d", d3dvp.Height);

        ImGui::EndGroup();

        if(pMainChar == NULL)
            pMainChar = GetMainCharacter();

        if(pMainChar){
            ImGui::BeginGroup();

            ImGui::Text(" # Main character information (%p):", pMainChar);
            ImGui::Separator();
            
            math::Vector vecMainChar = {
                *(float*)((DWORD*)pMainChar + 0x1A),
                *(float*)((DWORD*)pMainChar + 0x19),
                *(float*)((DWORD*)pMainChar + 0x1B)
            };

            ImGui::Text("Pos X: [%.5f] | Y: [%.5f] | Z: [%.5f]", vecMainChar.x, vecMainChar.y, vecMainChar.z);

            if(pCamMgr == nullptr)
                pCamMgr = CameraManager::GetInstance();

            if(pCamMgr != nullptr)
            {
                if(pCam == nullptr)
                    pCam = pCamMgr->GetCurrentInGameCamera();

                ImGui::Text(" # Camera manager information (%p):", pCamMgr);
                ImGui::Separator();

                //pure3d::Camera* pCam = pCamMgr->GetCurrentRenderCamera();
                pure3d::Camera* pCam = pCamMgr->GetCurrentInGameCamera();
                if(pCam != NULL)
                {
                    
                    if(thisCam == nullptr)
                        thisCam = reinterpret_cast<pure3d::VectorCamera*>(pCam);

                    ImGui::Text("Camera manager pCam (%p):", pCam);
                    pure3d::VectorCamera* thisCam = reinterpret_cast<pure3d::VectorCamera*>(pCam);
                    if(thisCam != NULL){
                            
                        static int id = 0;
                        ImGui::InputInt("Bone ID", &id);
                        math::Vector* bone_pos = pMainChar->GetJointPosition(id);
                        math::Vector boneScreen = p3dh->WorldToScreen(thisCam, d3dvp, bone_pos);
                        ImGui::GetBackgroundDrawList()->AddLine(ImVec2((boneScreen.x), (boneScreen.y)), ImVec2(d3dvp.Width/2, d3dvp.Height), ImGui::ColorConvertFloat4ToU32(ImVec4(255.0, 0.0, 255.0, 255.0)), 3);
                        ImGui::GetForegroundDrawList()->AddText({boneScreen.x,boneScreen.y}, -1, &std::to_string(id)[0]);

                        RenderSkeleton(pMainChar);
                        //for (auto i = 0; i < 24; i++) {
                        //    if (auto bone_pos = pMainChar->GetJointPosition(i)) {
                        //        math::Vector screen_posn;
                        //        thisCam->WorldToViewport(bone_pos, &screen_posn);

                        //        float x = (float(d3dvp.X) + (float(d3dvp.Width) / 2.0f)) + float(d3dvp.Width) * screen_posn.x;
                        //        float y = (float(d3dvp.Y) + (float(d3dvp.Height) / 2.0f)) + float(d3dvp.Height) * -screen_posn.y;

                        //        ImGui::GetForegroundDrawList()->AddText({x,y}, -1, &std::to_string(i)[0]);
                        //    }
                        //}
                    }

                    
                    

                    
                }
            } 
            ImGui::EndGroup();

            if(CVMgrObj == nullptr)
                CVMgrObj = CVManager::GetInstance();
            if(CVMgrObj != nullptr)
            {
                ImGui::BeginGroup();

                ImGui::Text(" # CV Manager information (%p):", CVMgrObj);
                ImGui::Separator();

                //int count = characters->GetCount();
                //for (int i = 0; i < count; i++)
                //{

                ImGui::EndGroup();
                GameSet<CharacterObject>* characters = CVManager::GetInstance()->GetCharacterSet();
                if(characters != nullptr)
                {
                    ImGui::Text(" Character set information (%p):", characters);
                    ImGui::Text(" Characters count (%d):", characters->mCount);
                    for (int i = 0; i < characters->mCount; i++)
                    {
                        if ( i >= characters->mCount )
                            break;
                        CharacterObject* character = characters->GetSet(i);

                        if(character != nullptr){
                            if(character == GetMainCharacter())
                                continue;
                                
                            RenderSkeleton(character);
                        }
                    }
                }
                pSpawnMixerObj = SpawnMixer::GetInstance();

                if(pSpawnMixerObj)
                {
                    ImGui::Text(" SpawnMixer information (%p):", pSpawnMixerObj);  
                    
                    static const char* current_item = 0;
                    static int eSpawnUsage = 0;
                    if(pLoadObject)
                    {
                        ImGui::Text(" CVLoadObject information (%p):", pLoadObject);
                        ImGui::Text(" CVLoadObject Template name (%s):", pLoadObject->m_Name);
                        
                        const std::map<const char*, ESpawnUsage> spawn_usage = {
                            {"None", ESpawnUsage::ESpawnUsage_None},
                            {"Group", ESpawnUsage::ESpawnUsage_Group},
                            {"Traffic", ESpawnUsage::ESpawnUsage_Traffic},
                            {"Ambient", ESpawnUsage::ESpawnUsage_Ambient},
                            {"Dealer", ESpawnUsage::ESpawnUsage_Dealer},
                            {"Gang", ESpawnUsage::ESpawnUsage_Gang},
                            {"Cop", ESpawnUsage::ESpawnUsage_Cop},
                            {"Mission", ESpawnUsage::ESpawnUsage_Mission},
                            {"NIS", ESpawnUsage::ESpawnUsage_NIS},
                            {"ANY", ESpawnUsage::ESpawnUsage_ANY}
                        };

                        if (ImGui::BeginCombo("#ESpawnUsage", current_item))
                        {
                            std::map<const char*, ESpawnUsage>::iterator it;
                            for (auto const& [key, val] : spawn_usage)
                            {
                                bool is_selected = (current_item == key);
                                if (ImGui::Selectable(key, is_selected)){
                                    current_item = key;
                                    eSpawnUsage = val;
                                }
                                if (is_selected)
                                    ImGui::SetItemDefaultFocus(); 
                            }
                            ImGui::EndCombo();
                        }
                        
                        ImGui::Text(" eSpawnUsage: (%d):", eSpawnUsage);
                    }

                    if(ImGui::Button("Spawn test"))
                    {
                        pLoadObject = pSpawnMixerObj->FindCVLoadObject("CharTemp_AA_Female01", ESpawnTemplateType::ESpawnTemplateType_Character);
                        if(pLoadObject){

                            SpawnObjectData spawnObjectData;
                        
                            spawnObjectData.mName = "CharacterTest";
                            spawnObjectData.mScriptClass = "";
                            spawnObjectData.mWeapon = "WeaponTemplateM16";
                            spawnObjectData.mTemplate =  pLoadObject->m_Name;
                            spawnObjectData.mTeam = "";
                           
                            math::Vector& position = *pMainChar->GetJointPosition(ESkeletonJoint::ESkeletonJoint_Character_Origin);
                            math::Vector direction = { 0, 0, 0 };

                            pLoadObject->AddSpawnUsage(eSpawnUsage);
                            pLoadObject->IsLoaded();

                            if(CharacterObject* pCharacter = (CharacterObject*)CVManager::GetInstance()->RequestCharacterSpawn(NULL, &spawnObjectData, position, direction))
                            {
                                pLoadObject->RemoveSpawnUsage(eSpawnUsage);
                            }
                        }
                    }
                    static bool bRenderCounts = false;

                    

                    if(ImGui::Button("Render counts"))
                        bRenderCounts = !bRenderCounts;

                    if(bRenderCounts)
                    {

                        ImGui::Text("m_HistoryCount (%d):", pLoadObject->m_HistoryCount);
                        ImGui::Text("m_SpawnLoadStatus (%d):", pLoadObject->m_SpawnLoadStatus);
                        ImGui::Text("m_SpawnStatus (%d):", pLoadObject->m_SpawnStatus);
                        ImGui::Text("m_GroupSpawnUsageCount (%d):", pLoadObject->m_GroupSpawnUsageCount);
                        ImGui::Text("m_TransitionalTrafficUsage (%d):", pLoadObject->m_TransitionalTrafficUsage);

                        ImGui::Text("m_TrafficSpawnUsageCount (%d):", pLoadObject->m_TrafficSpawnUsageCount);
                        ImGui::Text("m_NISSpawnUsageCount (%d):", pLoadObject->m_NISSpawnUsageCount);
                        ImGui::Text("m_GroupSpawnUsageCount (%d):", pLoadObject->m_GroupSpawnUsageCount);
                        ImGui::Text("m_AmbientSpawnUsageCount (%d):", pLoadObject->m_AmbientSpawnUsageCount);
                        ImGui::Text("m_DealerSpawnUsageCount (%d):", pLoadObject->m_DealerSpawnUsageCount);
                        ImGui::Text("m_GangSpawnUsageCount (%d):", pLoadObject->m_GangSpawnUsageCount);
                        ImGui::Text("m_CopSpawnUsageCount (%d):", pLoadObject->m_CopSpawnUsageCount);
                        ImGui::Text("m_MissionSpawnUsageCount (%d):", pLoadObject->m_MissionSpawnUsageCount);
                    }

                    
                    pDataBrokerObject = DataBroker::GetInstance();
                    
                    if(pDataBrokerObject)
                    {
                        ImGui::Text(" DataBroker information (%p):", pDataBrokerObject);  
                    
                        if(ImGui::Button("DataBroker LETTERBOX"))
                        {
                            pDataBrokerObject->Post(198, 0, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_RENDERFX 0"))
                        {
                            pDataBrokerObject->Post(207, 0, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_RENDERFX 1"))
                        {
                            pDataBrokerObject->Post(207, 1, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_ACTIVATE_PAUSE_PAGE 0"))
                        {
                            pDataBrokerObject->Post(208, 0, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_ACTIVATE_PAUSE_PAGE 1"))
                        {
                            pDataBrokerObject->Post(208, 1, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_MISSION_FAIL 0"))
                        {
                            pDataBrokerObject->Post(210, 0, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_MISSION_FAIL 1"))
                        {
                            pDataBrokerObject->Post(210, 1, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_GAME_OVER 0"))
                        {
                            pDataBrokerObject->Post(211, 0, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_GAME_OVER 1"))
                        {
                            pDataBrokerObject->Post(211, 1, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_PIMP_YOUR_MANSION 0"))
                        {
                            pDataBrokerObject->Post(212, 0, 0);
                        }
                        if(ImGui::Button("DataBroker DB_EVENT_PIMP_YOUR_MANSION 1"))
                        {
                            pDataBrokerObject->Post(212, 1, 0);
                        }
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