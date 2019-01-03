#include "clightmanual.h"
#include "alight.h"
/**
 * @brief CLightManual::CLightManual    Default constructor.
 */
CLightManual::CLightManual()
    : ALight ()
    , mCanTurnOn(false)
{
    this->mLightMode = LIGHT_MODE_MANUAL;
}

/**
 * @brief CLightManual::CLightManual    Constructor with arguments.
 * @param GpioPin           GPIO Pin number.
 * @param PinDirection      GPIO pin access direction, input, output, or both.
 * @param ChatteringTime    Chattering time. If the value is zero, chattering
 *                          will not be handled.
 * @param PeriodTime        Time to scan GPIO pin.
 * @param OnOffBorder       The border value the light state, ON or OFF, will be
 *                          changed. If the sensor sensor value is larger than this,
 *                          state of light is changed into ON, otherwise the state is
 *                          OFF.
 */
CLightManual::CLightManual(uint8_t GpioPin,
                           PART_PIN_DIRECTION PinDirection,
                           uint32_t ChatteringTime,
                           uint32_t PeriodTime,
                           uint32_t OnOffBorder)
    : ALight(GpioPin, PinDirection, ChatteringTime, PeriodTime, OnOffBorder)
    , mCanTurnOn(false)
{
    this->mLightMode = LIGHT_MODE_MANUAL;
}

/**
 * @brief CLightManual::Update  Update state of Light.
 * @param state The value to use to turn on or off the light, or the mode of light
 *              switch,
 */
void CLightManual::Update(int32_t state)
{
    if (this->mOnOffBorder < static_cast<uint32_t>(state)) {
        if (this->mCanTurnOn) {
            this->mLightState = LIGHT_STATE_ON;
        } else {
            this->mLightState = LIGHT_STATE_REQUEST;
        }
    } else {
        this->mLightState = LIGHT_STATE_OFF;
    }
}

/**
 * @brief CLightManual::Update  Update state which shows whether the light can be turned on
 *                              or not.
 *                              The state is changed each time when this method is called.
 */
void CLightManual::Update()
{
    this->mCanTurnOn = !this->mCanTurnOn;
}
