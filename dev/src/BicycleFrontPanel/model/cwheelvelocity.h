#ifndef CWHEELVELOCITY_H
#define CWHEELVELOCITY_H
#include "model/cwheel.h"

class CWheelVelocity : public CWheel
{
public:
    CWheelVelocity();
    CWheelVelocity(uint8_t GpioPin,
                   PART_PIN_DIRECTION PinDirection,
                   uint32_t ChatteringTime, uint32_t PeriodTime);
    ~CWheelVelocity() {}

    virtual void TimerCallback(int state);

    virtual std::string ToString();

public:
    uint32_t GetVelocity() { return this->mVelocity; }

protected:
    uint32_t mVelocity; //LSB = 0.1 [m]
};

#endif // CWHEELVELOCITY_H
