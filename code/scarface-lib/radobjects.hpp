#ifndef _RADOBJECT_HPP
#define _RADOBJECT_HPP

#include "radkey.hpp"

namespace core
{

    class IRefCount 
    {
    public:
        virtual void AddRef( void ) = 0;
        virtual void RemoveRef( void ) = 0;
        virtual int GetRefCount( void ) = 0;   
    };

}

namespace pure3d
{

    class Name
    {
        Name(const char* name)
        {
            m_uid = core::stxHashStringLowerCase(name, 0);
        }

        core::Key GetUID()
        {
            return m_uid;
        }
    private:
        core::Key m_uid;
    };

}

#endif // !_RADOBJECT_HPP