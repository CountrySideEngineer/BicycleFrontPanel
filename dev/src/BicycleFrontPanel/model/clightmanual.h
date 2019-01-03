#ifndef CLIGHTMANUAL_H
#define CLIGHTMANUAL_H
#include "alight.h"

class CLightManual : public ALight
{
public:
    CLightManual();
    CLightManual(uint8_t GpioPin,
                 PART_PIN_DIRECTION PinDirection,
                 uint32_t ChatteringTime,
                 uint32_t PeriodTime,
                 uint32_t OnOffBorder = 0);
    virtual ~CLightManual() {}

public: //Getter/Setter
    bool GetCanTurnOn() { return this->mCanTurnOn; }

public:
    virtual void Update(int32_t state);
    virtual void Update();

protected:
    bool mCanTurnOn;
};

#endif // CLIGHTMANUAL_H
