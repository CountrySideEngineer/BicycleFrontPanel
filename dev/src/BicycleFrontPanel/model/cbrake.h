#ifndef CBRAKE_H
#define CBRAKE_H
#include "model/apart.h"

class CBrake : public APart
{
public:
    CBrake();
    CBrake(uint8_t GpioPin,
           PART_PIN_DIRECTION PinDirection,
           uint32_t ChatteringTime,
           uint32_t PeridTime);
    virtual ~CBrake() {}

public: //Getter/Setter
    void SetIsHold(bool isHold) { this->mIsHold = isHold; }
    bool GetIsHold() const { return this->mIsHold; }

public:
    virtual void Update(int32_t State);
    virtual void Update();
    virtual void InterruptCallback(int state);

protected:
    bool mIsHold;
};

#endif // CBRAKE_H
