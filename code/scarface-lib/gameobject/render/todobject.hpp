#ifndef _TODOBJECT_HPP
#define _TODOBJECT_HPP

#include "gameobject/gamesetobject.hpp"

#pragma pack(push, 4)

class TODObject : public GameSetObject
{
public:
    virtual ~TODObject();

    static TODObject* GetInstance( void );

    //char gap4[44]; // ScriptObject, GameSetObject
    bool mPauseTimeOfDay;
    char gap31[3]; // can be solved with alignment of 8 but i am lazy
    float mTimeOfDaySpeed;
    unsigned int mHours;
    unsigned int mMinutes;
    float mPrelitLumTimes[6];
    int mHoldTimeLength[6];
    int mTransitionTimeLength[6];
    char *mAttachedClearEnvParam[6];
    char *mAttachedRainyEnvParam[6];
    bool mEnableRaining;
    char gapB9[3]; // can be solved with alignment of 8 but i am lazy
    float mRainPercentage;
    int mRainStartTransitFrames;
    int mRainEndTransitFrames;
    bool mApplyEnvDataOnly;
};

#pragma pack(pop)

static_assert(sizeof(TODObject) == 204, "TODObject has not been correctly aligned!");
const int x = sizeof(TODObject);

#endif // !_TODOBJECT_HPP