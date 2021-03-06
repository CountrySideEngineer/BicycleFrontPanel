#ifndef CBICYCLESTATE_H
#define CBICYCLESTATE_H
#include "model/apart.h"

class CBicycleState
{
public:
    enum BICYCLE_STATE_BRAKE {
        BICYCLE_STATE_BRAKE_OFF = 0,
        BICYCLE_STATE_BRAKE_FRONT,      //0x01
        BICYCLE_STATE_BRAKE_REAR,       //0x02
        BICYCLE_STATE_BRAKE_BOTH,       //0x03
        BICYCLE_STATE_BRAKE_MAX         //0x04
    };

    enum BICYCLE_STATE_LIGHT {
        BICYCLE_STATE_LIGHT_OFF = 0,
        BICYCLE_STATE_LIGHT_AUTO_ON,    //0x01
        BICYCLE_STATE_LIGHT_MANUAL_REQ, //0x02
        BICYCLE_STATE_LIGHT_MANUAL_ON,  //0x03
        BICYCLE_STATE_LIGHT_MAX         //0x04
    };

public:
    CBicycleState();
    ~CBicycleState() {}

public:
    void Update();
    void getState(BICYCLE_STATE_BRAKE* brakeState, BICYCLE_STATE_LIGHT* lightState);
    uint32_t getRotate();
    uint32_t getVelocity();
    void SwitchLightMode(int mode);

    std::string GetRotateValue();
    std::string GetVelocityValue();

protected:
    void Update(BICYCLE_STATE_BRAKE &brakeState);
    void Update(BICYCLE_STATE_LIGHT &brakeLight);
    void UpdateWheel();
    void UpdateWheel(APart* wheel);
    void ReadSPiData(APart* part);

protected:
    APart* mFrontBrake;
    APart* mRearBrake;
    APart* mFrontWheel;
    APart* mRearWheel;
    APart* mWheelVelocity;
    APart* mLight;

    BICYCLE_STATE_BRAKE mBrakeState;
    BICYCLE_STATE_LIGHT mLightState;
};

#endif // CBICYCLESTATE_H
