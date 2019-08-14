#include "clight.h"
#include <QtDebug>
#include "cgpio.h"
#include "model/apart.h"
#include "model/abicyclepart.h"

/**
 * @brief CLight::CLight    Default constructor.
 */
CLight::CLight()
    : ABicyclePart (nullptr, 0, PART_PIN_DIRECTION_MAX, 0, 0)
{}

/**
 * @brief CLight::CLight    Constructor with arguments.
 * @param GpioPin           GPIO pin number.
 * @param PinDirection      GPIO access direction, input, output, or both.
 * @param ChatteringTime    Chattering time. If this value is zero, chattering will not be
 *                          handled.
 * @param PeriodTime        Periodic time.
 */
CLight::CLight(uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : ABicyclePart(nullptr, GpioPin, PinDirection, ChatteringTime, PeriodTime) {}

/**
 * @brief CLight::CLight    Constructor with arguments.
 * @param model             Pointer to model, Qt's model-view architecture.
 * @param GpioPin           GPIO pin number.
 * @param PinDirection      GPIO access direction, input, output, or both.
 * @param ChatteringTime    Chattering time. If this value is zero, chattering will not be
 *                          handled.
 * @param PeriodTime        Periodic time.
 */
CLight::CLight(CBicycleItemModel* model,
               uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : ABicyclePart(model, GpioPin, PinDirection, ChatteringTime, PeriodTime) {}

/**
 * @brief CLight::CLight    Constructor with argument to set input and output pin.
 * @param model             Pointer to model, Qt's model-view architecture.
 * @param InputPin          GPIO pin No. for input.
 * @param OutputPin         GPIO pin No. for output.
 * @param ChatteringTime    Chattering time. If this value is zero, chattering will not be
 *                          handled.
 * @param PeriodTime        Periodic time.
 */
CLight::CLight(CBicycleItemModel* model,
               uint8_t InputPin,
               uint8_t OutputPin,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : ABicyclePart(model, InputPin, OutputPin, ChatteringTime, PeriodTime) {}

/**
 * @brief CLight::~CLight   Destructor.
 */
CLight::~CLight()
{
    CGpio* instance = CGpio::GetInstance();
    instance->GpioWrite(this->GetPin(), CGpio::GPIO_PIN_LEVEL_LOW);
    instance->GpioWrite(this->GetOptionPin(), CGpio::GPIO_PIN_LEVEL_LOW);
}

/**
 * @brief CLight::InterruptCallback Update request state from light, on or off.
 * @param state State of request on or off.
 */
void CLight::InterruptCallback(int state)
{
    this->Update(state);
}

/**
 * @brief CLight::Update    Update light state of view and device.
 * @param state             State of light to be updated.
 */
void CLight::Update(int32_t state)
{
    try {
        this->mModel->setData(this->mInputPin, static_cast<uint32_t>(state));
    }
    catch (exception &ex) {
        cout << ex.what() << endl;
    }
}

void CLight::UpdateState(uint32_t state)
{
    if (!(PART_PIN_DIRECTION_OUTPUT & this->mPinDirection)) {
        /*
         * @ToDo:Throw exception.
         */
        return;
    } else {
        CGpio* instance = CGpio::GetInstance();
        instance->GpioWrite(this->mOutputPin, static_cast<uint8_t>(state));
    }
}
