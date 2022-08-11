#ifndef _MOD_HPP
#define _MOD_HPP

namespace gangsta
{

    class CMod
    {
    public:
        void InitSafe();
        void Init();
        void RunLoop();
    };

    inline CMod g_Mod;

}

#endif