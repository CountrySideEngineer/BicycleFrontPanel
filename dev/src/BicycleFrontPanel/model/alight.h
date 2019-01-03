#ifndef ALIGHT_H
#define ALIGHT_H
#include "model/apart.h"

/**
 * @brief The ALight class  Abstracting class of light part.
 */
class ALight : public APart
{
public:
    enum LIGHT_STATE {
        LIGHT_STATE_OFF = 0,
        LIGHT_STATE_ON,
        LIGHT_STATE_REQUEST,
        LIGHT_STATE_MAX
    };
    enum LIGH_MODE {
        LIGHT_MODE_AUTO = 0,
        LIGHT_MODE_MANUAL,
        LIGHT_MODE_MAX
    };

public:
    ALight();
    ALight(uint8_t GpioPin,
           PART_PIN_DIRECTION PinDirection,
           uint32_t ChatteringTime,
           uint32_t PeriodTime,
           uint32_t OnOffBorder = 0);

public: //Getter/Setter
    LIGHT_STATE GetLightState() { return this->mLightState; }
    LIGH_MODE   GetLightMode() { return this->mLightMode; }

    uint32_t GetOnOffBorder() { return this->mOnOffBorder; }
    void SetOnOffBorder(uint32_t OnOffBorder) { this->mOnOffBorder = OnOffBorder; }

protected:
    LIGHT_STATE mLightState;
    LIGH_MODE   mLightMode;

    uint32_t    mOnOffBorder;
};

#endif // ALIGHT_H
