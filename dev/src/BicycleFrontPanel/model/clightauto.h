#ifndef CLIGHTAUTO_H
#define CLIGHTAUTO_H
#include "alight.h"

class CLightAuto : public ALight
{
public:
    CLightAuto();
    CLightAuto(uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime,
               uint32_t OnOffBorder = 0);
    virtual ~CLightAuto() {}

    virtual void Update(int32_t state);
    virtual void Update();
};

#endif // CLIGHTAUTO_H
