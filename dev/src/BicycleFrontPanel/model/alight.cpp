#include "alight.h"

/**
 * @brief ALight::ALight    Default constructor.
 */
ALight::ALight()
    : APart(0, PART_PIN_DIRECTION_MAX, 0, 0)
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
 */
ALight::ALight(uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : APart(GpioPin, PinDirection, ChatteringTime, PeriodTime)
{
    this->mLightState = LIGHT_STATE_OFF;
    this->mLightMode = LIGHT_MODE_AUTO;
}
