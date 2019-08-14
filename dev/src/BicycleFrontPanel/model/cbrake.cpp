#include <QObject>
#include "QtDebug"
#include "cgpio.h"
#include "model/apart.h"
#include "model/cbrake.h"

/**
 * @brief CBrake::CBrake    Default constructor.
 */
CBrake::CBrake()
    : ABicyclePart(nullptr, 0, PART_PIN_DIRECTION_MAX, 0, 0)
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
    : ABicyclePart(nullptr, GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mIsHold(false)
{}

CBrake::CBrake(CBicycleItemModel* model,
               uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : ABicyclePart(model, GpioPin, PinDirection, ChatteringTime, PeriodTime)
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
    bool dataToSet = false;
    if (0 == state) {
        dataToSet = false;
    } else {
        dataToSet = true;
    }

    this->mModel->setData(this->mInputPin, dataToSet);
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

/**
 * @brief CBrake::Initialize    Initialize data by data read from GPIO pin.
 */
void CBrake::Initialize()
{
    CGpio* instance = CGpio::GetInstance();

    uint8_t level = 0;
    instance->GpioRead(this->mInputPin, &level);
    this->Update(level);
}
