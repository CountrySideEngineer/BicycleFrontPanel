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

/**
 * @brief InterruptCallback Callback function called when interrupt has been
 *                          detectec. This function is called immediately if
 *                          member varibable mChatteringTime is zero, otherwise
 *                          called after mChatteringTime passed.
 * @param state Pin level, 0 mean LOW, othervalue is HIGH.
 */
void APart::InterruptCallback(int /* state */) {}


/**
 * @brief APart::TimerCallback  Callback function called periodically. The period is
 *                              set by member variable mPeriodTime. If the value is
 *                              zero, this fuction is never called.
 * @param state Pin level, 0 mean LOW, othervalue is HIGH.
 */
void APart::TimerCallback(int /* state */) {}
