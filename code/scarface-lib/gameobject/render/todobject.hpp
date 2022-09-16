#ifndef _TODOBJECT_HPP
#define _TODOBJECT_HPP

#include "gameobject/gamesetobject.hpp"

#pragma pack(push, 4)

class TODObject : public GameSetObject
{
public:
    virtual ~TODObject() = 0;

    static TODObject* GetInstance( void );

    bool mPauseTimeOfDay;
    float mTimeOfDaySpeed;
    unsigned int mHours;
    unsigned int mMinutes;
    float mPrelitLumTimes[6];
    int mHoldTimeLength[6];
    int mTransitionTimeLength[6];
    char *mAttachedClearEnvParam[6];
    char *mAttachedRainyEnvParam[6];
    bool mEnableRaining;
    float mRainPercentage;
    int mRainStartTransitFrames;
    int mRainEndTransitFrames;
    bool mApplyEnvDataOnly;
};

#pragma pack(pop)

static_assert(sizeof(TODObject) == 204, "TODObject has not been correctly aligned!");

#endif // !_TODOBJECT_HPP