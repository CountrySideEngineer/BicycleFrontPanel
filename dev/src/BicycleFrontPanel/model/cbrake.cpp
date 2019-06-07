#include <QObject>
#include "QtDebug"
#include "cgpio.h"
#include "model/apart.h"
#include "model/cbrake.h"

/**
 * @brief CBrake::CBrake    Default constructor.
 */
CBrake::CBrake()
    : APart(0, PART_PIN_DIRECTION_MAX, 0, 0)
    , mIsHold(false) {}

/**
 * @brief CBrake::CBrake    Constructor with arguments.
 * @param GpioPin           GPIO pin number.
 * @param PinDirection      GPIO access direction, input, output, or both.
 * @param ChatteringTime    Chattering time. If this value is zero, chattering
 *                          will not be handled.
 * @param PeriodTime        Periodic time.
 */
CBrake::CBrake(uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : APart(GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mIsHold(false)
{}

/**
 * @brief CBrake::~CBrake   Destructor.
 */
CBrake::~CBrake()
{
    CGpio* instance = CGpio::GetInstance();
    instance->GpioWrite(this->GetOptionPin(), CGpio::GPIO_PIN_LEVEL_LOW);
}

/**
 * @brief CBrake::InterruptCallback Callback function to be called when interrupt occurred.
 */
void CBrake::InterruptCallback(int state)
{
    this->Update(state);
}

/**
 * @brief CBrake::Update    Update state of brake.
 * @param state State of brake, read from GPIO pin. This value means pin level,
 *              0 is LOW, others are HIHG.
 */
void CBrake::Update(int32_t state)
{
    CGpio* instance = CGpio::GetInstance();
    if (0 == state) {
        /*
         * The pin level low means the sensor is near to magnet.
         */
        this->mIsHold = true;

        instance->GpioWrite(this->GetOptionPin(), CGpio::GPIO_PIN_LEVEL_HIGH);
    } else {
        /*
         * The pin level low means the sensor is far from magnet.
         */
        this->mIsHold = false;

        instance->GpioWrite(this->GetOptionPin(), CGpio::GPIO_PIN_LEVEL_LOW);
    }
}

/**
 * @brief CWheel::Update    !!!ATTENTION!!! Update nothing in this class.
 */
void CBrake::Update() {}

/**
 * @brief CBrake::SetOptionPin  Set optional GPIO pin number.
 * @param optionPin     The GPIO pin number to be used optinally.
 */
void CBrake::SetOptionPin(uint8_t optionPin)
{
    APart::SetOptionPin(optionPin);

    CGpio* instance = CGpio::GetInstance();
    instance->SetMode(this->GetOptionPin(), CGpio::GPIO_PIN_DIRECTION_OUTPUT);
}
