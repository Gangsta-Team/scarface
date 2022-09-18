#ifndef _GAMESETOBJECT_HPP
#define _GAMESETOBJECT_HPP

#include "scriptobject.hpp"

#pragma pack(push, 1)

class GameSetObject : public ScriptObject
{
public:
    virtual ~GameSetObject();

    uint8_t gap1F;
    char char20[0x10];
};

#pragma pack(pop)

static_assert(sizeof(GameSetObject) == 0x30);

#endif // !_GAMESETOBJECT_HPP