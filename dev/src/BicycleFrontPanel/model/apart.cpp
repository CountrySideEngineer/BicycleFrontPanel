#include "apart.h"

/**
 * @brief APart::APart  Default constructor
 */
APart::APart()
    : mState(0)
    , mGpioPin(0)
    , mChatteringTime(0)
    , mPeriodTime(0)
    , mIsFailure(false)
    , mFailureCode(0x0000)
    , mPinDirection(PART_PIN_DIRECTION_INPUT)
{}

/**
 * @brief APart::APart  Constructor with argumetn to initialize GPIO pin number, that of direction,
 *                      input, output, or both, and chattering time to handle chattering.
 * @param GpioPin   GPIO pin no.
 * @param PinDirection  GPIO pin access direction, input, output, or both.
 * @param ChatteringTime    Time to wait until chattering finish.
 */
APart::APart(uint8_t GpioPin, PART_PIN_DIRECTION PinDirection, uint32_t ChatteringTime, uint32_t PeriodTime)
    : mState(0)
    , mGpioPin(GpioPin)
    , mChatteringTime(ChatteringTime)
    , mPeriodTime(PeriodTime)
    , mIsFailure(false)
    , mFailureCode(0x0000)
    , mPinDirection(PinDirection)
{}

/**
 * @brief APart::~APart Destructor.
 */
APart::~APart() {}

void APart::InterruptCallback(int /* state */) {}
void APart::TimerCallback(int /* state */) {}
