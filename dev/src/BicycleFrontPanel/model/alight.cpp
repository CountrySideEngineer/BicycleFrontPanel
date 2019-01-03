#include "alight.h"

/**
 * @brief ALight::ALight    Default constructor.
 */
ALight::ALight()
    : APart(0, PART_PIN_DIRECTION_MAX, 0, 0)
    , mOnOffBorder(0)
{
    this->mLightState = LIGHT_STATE_OFF;
    this->mLightMode = LIGHT_MODE_AUTO;
}

/**
 * @brief ALight::ALight    Constructor with argument.
 * @param GpioPin           GPIO pin number.
 * @param PinDirection      GPIO access direction, input, output, ot both.
 * @param ChatteringTime    Chattering time. If this value is zero, chattering
 *                          will not be handled.
 * @param PeriodTime        Time to scan GPIO pin.
 * @param OnOffBorder       The border value the light state, ON or OFF, will be
 *                          changed. If the sensor sensor value is larger than this,
 *                          state of light is changed into ON, otherwise the state is
 *                          OFF.
 */
ALight::ALight(uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime,
               uint32_t OnOffBorder)
    : APart(GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mOnOffBorder(OnOffBorder)
{
    this->mLightState = LIGHT_STATE_OFF;
    this->mLightMode = LIGHT_MODE_AUTO;
}
