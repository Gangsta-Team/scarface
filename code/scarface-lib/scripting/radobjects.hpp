#ifndef _RADOBJECT_HPP
#define _RADOBJECT_HPP

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

#endif // !_RADOBJECT_HPP