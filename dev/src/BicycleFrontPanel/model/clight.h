#ifndef CLIGHT_H
#define CLIGHT_H
#include "model/abicyclepart.h"

class CLight : public ABicyclePart
{
public:
    enum LIGHT_SETTING {
        LIGHT_SETTING_MODE = 0,
        LIGHT_SETTING_STATE,
        LIGHT_SETTING_MAX
    };

public:
    CLight();
    CLight(uint8_t GpioPin,
           PART_PIN_DIRECTION PinDirection,
           uint32_t ChatteringTime = 0,
           uint32_t PeriodTimer = 0);
    CLight(CBicycleItemModel* model,
           uint8_t GpioPin,
           PART_PIN_DIRECTION PinDirection,
           uint32_t ChatteringTime = 0,
           uint32_t PeriodTimer = 0);
    CLight(CBicycleItemModel* model,
           uint8_t InputPin,
           uint8_t OutputPin,
           uint32_t ChatteringTime = 0,
           uint32_t PeriodTimer = 0);
    virtual ~CLight() override;

public:
    virtual void Update(int32_t state) override;
    virtual void Update() override {}
    virtual void InterruptCallback(int state) override;
    virtual void UpdateState(uint32_t state);

protected:
    virtual void UpdateMode(uint16_t mode) {}
};

#endif // CLIGHT_H
