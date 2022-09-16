#include "todobject.hpp"
#include "common.hpp"

static TODObject** s_TODSingleton = (TODObject**)0x0082B384;

TODObject* TODObject::GetInstance()
{
    return (*s_TODSingleton);
}