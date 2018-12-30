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
               uint32_t PeridTime,
               uint32_t OnOffBorder = 0);
    virtual ~CLightAuto() {}

    virtual void Update(int32_t state);
    virtual void Update();

    uint32_t GetOnOffBorder() { return this->mOnOffBorder; }
    void SetOnOffBorder(uint32_t OnOffBorder) { this->mOnOffBorder = OnOffBorder; }

protected:
    uint32_t    mOnOffBorder;

};

#endif // CLIGHTAUTO_H
