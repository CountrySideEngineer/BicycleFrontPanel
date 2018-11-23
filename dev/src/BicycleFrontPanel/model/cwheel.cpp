#include <iostream>
#include "model/cwheel.h"
using namespace std;

/**
 * @brief CWheel::CWheel    Default constructor.
 */
CWheel::CWheel()
    : APart(0, PART_PIN_DIRECTION_MAX, 0, 0)
    , mRpm(0)
{}

/**
 * @brief CWheel::CWheel    Constructor with arguments.
 * @param GpioPin           GPIO pin number.
 * @param PinDirection      GPIO pin access direction, input, output, or both edge.
 * @param ChatteringTime    Chattering time.
 */
CWheel::CWheel(uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : APart(GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mRpm(0)
{}

/**
 * @brief CWheel::InterruptCallback Callback function to be called when interrupt occurred.
 */
void CWheel::InterruptCallback(int /* state */)
{
    this->Update();
}

/**
 * @brief CWheel::TimerCallback Callback function to be called when the time dispatched.
 */
void CWheel::TimerCallback(int /* state */)
{
    this->mRpm = (static_cast<uint16_t>(this->mState)  * 60) / mInterval;
    this->mState = 0;
}

/**
 * @brief CWheel::Update    Update rotate count.
 */
void CWheel::Update()
{
    this->mState++;
}

/**
 * @brief CWheel::Update    !!!ATTENTION!!! Update nothing in this class.
 */
void CWheel::Update(int32_t /* state */) {}
