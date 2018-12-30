#include "clightauto.h"

/**
 * @brief CLightAuto::CLightAuto    Default constructor.
 */
CLightAuto::CLightAuto()
    : ALight()
    , mOnOffBorder(0)
{
    this->mLightMode = LIGHT_MODE_AUTO;
}

/**
 * @brief CLightAuto::CLightAuto    Constructor with arguments
 * @param GpioPin           GPIO pin number.
 * @param PinDirection      GPIO access direction, input, output, ot both.
 * @param ChatteringTime    Chattering time. If this value is zero, chattering
 *                          will not be handled.
 * @param PeriodTime        Time to scan GPIO pin.
 */
CLightAuto::CLightAuto(uint8_t GpioPin,
                       PART_PIN_DIRECTION PinDirection,
                       uint32_t ChatteringTime,
                       uint32_t PeriodTime,
                       uint32_t OnOffBorder)
    : ALight(GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mOnOffBorder(OnOffBorder)
{
    this->mLightMode = LIGHT_MODE_AUTO;
}

/**
 * @brief CLightAuto::Update    Update state of light.
 * @param State State of light, ON or OFF.
 */
void CLightAuto::Update(int32_t state)
{
    if (this->mOnOffBorder < static_cast<uint32_t>(state)) {
        this->mLightState = LIGHT_STATE_ON;
    } else {
        this->mLightState = LIGHT_STATE_OFF;
    }
}

/**
 * @brief CWheel::Update    !!!ATTENTION!!! Update nothing in this class.
 */
void CLightAuto::Update() {}
