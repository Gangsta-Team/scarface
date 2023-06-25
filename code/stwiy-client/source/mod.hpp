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
        void InitImGuiStyle();
        void RunLoop();
        void RunGui(bool* pGui, HWND hMainWindow);
        void RenderSkeleton(CharacterObject *character);
        void InputWatcher(HWND hMainWindow);
    };

    inline CMod g_Mod;

}

extern Client* client;

#endif