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

#include "packets.hpp"

pure3dH                 *p3dh;
D3DVIEWPORT9            d3dvp;
CharacterObject         *pMainChar = nullptr;

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

void CVM_SetMainCharacterPackage(const char* package)
{
    char buff[256];
    sprintf(buff,"CVM_SetMainCharacterPackage(\"%s\");", package);
    //Con_evaluate(buff);
    gangsta::CHooks::Con_evaluate(buff, 0, 0, 0, -1);
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


OnFootSyncPacket CreateOnFootSyncPacket(float posX, float posY, float posZ,
                                        float rotationPitch, float rotationYaw, float rotationRoll,
                                        int animationState, int equipmentState, int skin) {
    OnFootSyncPacket syncPacket;
    syncPacket.posX = posX;
    syncPacket.posY = posY;
    syncPacket.posZ = posZ;
    syncPacket.rotationPitch = rotationPitch;
    syncPacket.rotationYaw = rotationYaw;
    syncPacket.rotationRoll = rotationRoll;
    syncPacket.animationState = animationState;
    syncPacket.equipmentState = equipmentState;
    syncPacket.skinId = skin;
    return syncPacket;
}

void SendPacket(ENetPeer* peer, const RPCPacket& syncPacket) {
    ENetPacket* packet = enet_packet_create(&syncPacket, sizeof(RPCPacket), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    enet_host_flush(peer->host);
}

bool once = true;
void gangsta::CMod::RunGui(bool* pGui, HWND hMainWindow)
{
    if(ImGui::Begin("Debug"))
    {

        if(once)
        {
            
            RPCPacket syncPacket;
            syncPacket.action = RPCAction::Interact;
            syncPacket.playerId = 1;
            // Set additional data fields as needed

            client->CreatePacket(syncPacket);

            once = false;

        }
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
            
            //packet_pos.position.x = *(float*)((DWORD*)pMainChar + 0x1A);
            //packet_pos.position.y = *(float*)((DWORD*)pMainChar + 0x19);
            //packet_pos.position.z = *(float*)((DWORD*)pMainChar + 0x1B);

            ImGui::Text("Pos X: [%.5f] | Y: [%.5f] | Z: [%.5f]", vecMainChar.x, vecMainChar.y, vecMainChar.z);
        
            ImGui::Text("Skin: [%d] %s ", current_idx, szCharacters[current_idx]);

            RPCPacket packet;
            OnFootSyncPacket onFootSync = CreateOnFootSyncPacket(vecMainChar.x, vecMainChar.y, vecMainChar.z, 50.0f, 50.0f, 50.0f, 1, 2, current_idx);
            RPCPacket rpcPacket;
            rpcPacket.action = RPCAction::Walk;
            rpcPacket.playerId = 0;
            rpcPacket.onFootSync = onFootSync;
            
            client->SendPacket(rpcPacket);

            if(ImGui::Button("<"))
            {
                current_idx--;
                if(current_idx < 0)
                    current_idx = 15;
                CVM_SetMainCharacterPackage(szCharacters[current_idx]);
            }
            ImGui::SameLine();
            if(ImGui::Button("SELECT"))
            {
                //packet.index = current_idx;
                RPCPacket packet;
                OnFootSyncPacket onFootSync = CreateOnFootSyncPacket(vecMainChar.x, vecMainChar.y, vecMainChar.z, 50.0f, 50.0f, 50.0f, 1, 2, current_idx);
                RPCPacket rpcPacket;
                rpcPacket.action = RPCAction::Walk;
                rpcPacket.playerId = 0;
                rpcPacket.onFootSync = onFootSync;
                
                client->SendPacket(rpcPacket);
            }
            ImGui::SameLine();
            if(ImGui::Button(">"))
            {
                current_idx++;
                if(current_idx == 16)
                    current_idx = 0;

                CVM_SetMainCharacterPackage(szCharacters[current_idx]);
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