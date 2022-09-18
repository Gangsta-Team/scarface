#ifndef _SCRIPTOBJECT_HPP
#define _SCRIPTOBJECT_HPP

#include "common.hpp"
#include "radKey.hpp"

#pragma pack(push, 1)

class ConsoleObject
{
public:
    virtual ~ConsoleObject() = 0;
};

class ScriptObject : public ConsoleObject
{
public:
    virtual void _UnkInstance() = 0;
    virtual ~ScriptObject() = 0;
    virtual bool HasAName() = 0;
    virtual core::Key SetName(const char* scriptObjectName) = 0;
    virtual void ApplyChanges(bool onInit) = 0;
    
    // Needs to be reversed.
    virtual void _Unk1() = 0;
    virtual const char* ExecuteFromObject(char* eval) = 0;
    
    // More Functions after that but they still need to be reverse engineered.


    const char* objectName;
    uint32_t _t8;
    core::Key objectNameHash; // dwordC
    uint32_t _t10;
    uint32_t _t14;
    uint32_t _t18;
    uint16_t _t1C;
    uint8_t objectFlags;
};

#pragma pack(pop)

static_assert(sizeof(ConsoleObject) == 0x4);
static_assert(sizeof(ScriptObject) == 0x1f);

#endif // !_SCRIPTOBJECT_HPP