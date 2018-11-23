#ifndef CWHEEL_H
#define CWHEEL_H
#include <iostream>
#include "model/apart.h"

/**
 * @brief The CWheel class  The class of wheel of bicycle.
 */
class CWheel : public APart
{
public:
    explicit CWheel();
    explicit CWheel(uint8_t GpioPin,
                    PART_PIN_DIRECTION PinDirection,
                    uint32_t ChatteringTime,
                    uint32_t PeriodTime);
    virtual ~CWheel() {}

    virtual void Update(int32_t state);
    virtual void Update();
    virtual void InterruptCallback(int state);
    virtual void TimerCallback(int state);

public: //Getter/Setter
    uint32_t GetRpm() { return this->mRpm; }

protected:
    uint32_t mRpm;
    uint64_t mTmpCounter;

    const uint16_t mInterval = 5000;    //Watch dog timer interval.
};

#endif // CWHEEL_H
