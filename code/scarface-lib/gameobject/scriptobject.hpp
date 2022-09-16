#ifndef _SCRIPTOBJECT_HPP
#define _SCRIPTOBJECT_HPP

#include "common.hpp"

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
    virtual ~ScriptObject() = default;
    virtual bool _Unk0() = 0;

    uint32_t _t4;
    uint32_t _t8;
    uint32_t _tC;
    uint32_t _t10;
    uint32_t _t14;
    uint32_t _t18;
    uint16_t _t1C;
    uint8_t _t1E;
};

#pragma pack(pop)

static_assert(sizeof(ConsoleObject) == 0x4);
static_assert(sizeof(ScriptObject) == 0x1f);

#endif // !_SCRIPTOBJECT_HPP