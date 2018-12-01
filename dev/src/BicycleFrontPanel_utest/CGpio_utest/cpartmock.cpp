#include "cpartmock.h"

/**
 * @brief CPartMock::CPartMock  Default constructor
 */
CPartMock::CPartMock()
    : APart()
{
    this->mInterruptCallbackCalledCount = 0;
    for (int index = 0; index < 1024; index++) {
        this->mInterruptCallback1StArg[index] = 0;
    }
}

/**
 * @brief CPartMock::CPartMock  Constructor with initial value.
 * @param GpioPin           GPIO pin numebr.
 * @param PinDirection      GPIO pin access direction.
 * @param ChatteringTime    Time to wait for Chattering.
 * @param PeridTime         Periodic time.
 */
CPartMock::CPartMock(uint8_t GpioPin,
                     PART_PIN_DIRECTION PinDirection,
                     uint32_t ChatteringTime,
                     uint32_t PeridTime)
    : APart(GpioPin, PinDirection, ChatteringTime, PeridTime)
{
    this->mInterruptCallbackCalledCount = 0;
    for (int index = 0; index < 1024; index++) {
        this->mInterruptCallback1StArg[index] = 0;
    }
}

/**
 * @brief CPartMock::InterruptCallback  InterruptCallback method mock.
 * @param state Parameter abstracting state of GPIO pin level.
 */
void CPartMock::InterruptCallback(int state)
{
    this->mInterruptCallback1StArg[this->mInterruptCallbackCalledCount] = state;
    this->mInterruptCallbackCalledCount++;
}
