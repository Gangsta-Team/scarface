#include "common.hpp"
#include "mod.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "config.hpp"
#include "hooks.hpp"

#include "utils/ImGuiExtras.hpp"

#include <imgui.h>
#include <radkey.hpp>

#include "d3d9/Direct3DDevice9Proxy.h"
#include "d3d9/Direct3DProxy.h"
#include "d3d9/d3dx_include.h"

#include "gameutils/pure3dHelper.hpp"

pure3dH                 *p3dh;
D3DVIEWPORT9            d3dvp;
CharacterObject         *pMainChar = nullptr;
CVManager               *pCVMgr = nullptr;

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

const char* szCharacters[] = {
	"MCP_ArmyTony",
	"MCP_Assassin",
	"MCP_BlackSuitTony",
	"MCP_BluePinSuitShadesTony",
	"MCP_BluePinSuitTony",
	"MCP_BlueSuitTony",
	"MCP_Driver",
	"MCP_Enforcer",
	"MCP_GraySuitShadesTony",
	"MCP_GraySuitTony",
	"MCP_HawaiianShadesTony",
	"MCP_HawaiianTony",
	"MCP_SandyShadesTony",
	"MCP_SandyTony",
	"MCP_WhiteSuitShadesTony",
	"MCP_WhiteSuitTony",
};

int current_idx = 0;
int old_idx = 0;
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

        if(pMainChar == NULL)
            pMainChar = GetMainCharacter();

        if(pMainChar){

            ImGui::Text(" # Main character information (%p):", pMainChar);
            ImGui::Separator();
            
            math::Vector vecMainChar = {
                *(float*)((DWORD*)pMainChar + 0x1A),
                *(float*)((DWORD*)pMainChar + 0x19),
                *(float*)((DWORD*)pMainChar + 0x1B)
            };

            ImGui::Text("Pos X: [%.5f] | Y: [%.5f] | Z: [%.5f]", vecMainChar.x, vecMainChar.y, vecMainChar.z);
        
            if(pCVMgr == NULL)
                pCVMgr = CVManager::GetInstance();

            if(pCVMgr)
            {
                ImGui::Text(" # CVManager information (%p):", pCVMgr);
                ImGui::Text("Skin: [%d] %s ", current_idx, gUsingPackage.c_str());
                
                if(ImGui::Button("<"))
                {
                    current_idx--;
                    if(current_idx < 0)
                        current_idx = 15;

                    pCVMgr->CVM_SetMainCharacterPackage((char*)szCharacters[current_idx], 0);
                }
                ImGui::SameLine();
                if(ImGui::Button(">"))
                {
                    current_idx++;
                    if(current_idx == 16)
                        current_idx = 0;

                    pCVMgr->CVM_SetMainCharacterPackage((char*)szCharacters[current_idx], 0);
                }
            }
        }

        ImGui::EndGroup();
    }
    ImGui::End();

    // pGui = is opened
    if(*pGui)
    {
        gangsta::g_Mod.InputWatcher(hMainWindow);
    }
}