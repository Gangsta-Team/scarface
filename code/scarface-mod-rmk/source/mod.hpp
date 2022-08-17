#ifndef _MOD_HPP
#define _MOD_HPP

#include "common.hpp"

namespace gangsta
{

    class CMod
    {
    public:
        void InitSafe();
        void Init();
        void RunLoop();
        void RunGui(bool* pGui, HWND hMainWindow);
        void InputWatcher(HWND hMainWindow);
    };

    inline CMod         g_Mod;
    inline TextEditor*  g_TE;
    inline ImGui::FileBrowser fileDialog;

}


#endif