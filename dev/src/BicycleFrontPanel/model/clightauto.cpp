#include "clightauto.h"

/**
 * @brief CLightAuto::CLightAuto    Default constructor.
 */
CLightAuto::CLightAuto()
    : ALight()
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
 * @param OnOffBorder       The border value the light state, ON or OFF, will be
 *                          changed. If the sensor sensor value is larger than this,
 *                          state of light is changed into ON, otherwise the state is
 *                          OFF.
 */
CLightAuto::CLightAuto(uint8_t GpioPin,
                       PART_PIN_DIRECTION PinDirection,
                       uint32_t ChatteringTime,
                       uint32_t PeriodTime,
                       uint32_t OnOffBorder)
    : ALight(GpioPin, PinDirection, ChatteringTime, PeriodTime, OnOffBorder)
{
    this->mLightMode = LIGHT_MODE_AUTO;
}

/**
 * @brief CLightAuto::Update    Update state of light.
 * @param state The value to use to turn on or off the light.
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
