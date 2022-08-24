#ifndef _RADOBJECT_HPP
#define _RADOBJECT_HPP

struct IRefCount 
{
    friend void radAddRef( IRefCount * pRefObject, void * pParentObject );
    friend void radRelease( IRefCount * pRefObject, void * pParentObject );
    //
    // Invoke this member anytime another object is given a pointer to this object.
    // Simply updates the reference count.
    //
    virtual void AddRef( void ) = 0;

    //
    // This member should be used instead of delete. It will ensure correct management
    // of the objects lifetime.
    //
    virtual void Release( void ) = 0;   
};

inline void radAddRef( IRefCount * pRefObject, void * pParentObject )
{
#if defined RADMEMORYMONITOR
    radMemoryMonitorReportAddRefAdaptor( pParentObject, pRefObject );
#endif
    (void)pParentObject;
    pRefObject->AddRef( );
}

inline void radRelease( IRefCount * pRefObject, void * pParentObject )
{
#if defined RADMEMORYMONITOR
    radMemoryMonitorReportReleaseAdaptor( const_cast< void *>( pParentObject ), pRefObject );
#endif
    (void)pParentObject;
    pRefObject->Release( );
}

#endif // !_RADOBJECT_HPP