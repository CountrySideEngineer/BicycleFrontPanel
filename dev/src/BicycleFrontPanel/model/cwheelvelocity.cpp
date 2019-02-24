#include <iostream>
#include "cwheelvelocity.h"
using namespace std;

CWheelVelocity::CWheelVelocity() : CWheel () {}
CWheelVelocity::CWheelVelocity(uint8_t GpioPin,
                               PART_PIN_DIRECTION PinDirection,
                               uint32_t ChatteringTime, uint32_t PeriodTime)
    : CWheel (GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mVelocity(0)
{}

#define PI                  (314)       //LSB = 0.01
#define INCH_PER_MM         (254)       //LSB = 0.1 mm
#define WHEEL_R_INCH        (26)        //LSB = 1.0
#define WHEEL_R_MM      ((WHEEL_R_INCH) * (INCH_PER_MM))    //LSB = 0.01 * 0.1 = 1.0 * 10^(-3) mm
//#define WHEEL_CIRCUMFERENCE     WHEEL_R_MM
/*
 * The macro WHEEL_CIRCUMFERENCE shows circumference of wheel whose diameter
 * is 26 inch, about 2 meter.
 * The value is calcurated by 3.14 * 25.4 * 26 / 1000
 */
#define WHEEL_CIRCUMFERENCE (2073)     //LSB = 0.001 [m]
/**
 * @brief CWheelVelocity::UpdateView    Update view.
 */
void CWheelVelocity::TimerCallback(int state)
{
    CWheel::TimerCallback(state);

    uint64_t velocity = WHEEL_CIRCUMFERENCE * this->mRpm;   //Velocity per minute.
    this->mVelocity = (velocity * 60) / 100;    //LSB = 0.1 [m/h]
}

/**
 * @brief CWheelVelocity::ToString  Returns the velocity in string type.
 *                                  The value has fixed point data, its LSB is 0.1[m/h].
 * @return Velocity value whose lsb is 0.1[m/s].
 */
string CWheelVelocity::ToString()
{
    int integerPart = 0;
    int decimalPart = 0;

    /*
     * The value mVelocity has 0.1[m/s] lsb.
     * So, to get the string type value, the integer and decimal value should be
     * calcurated each other.
     */
    integerPart = this->mVelocity / 10;
    decimalPart = this->mVelocity % 10;

    return to_string(integerPart) + "." + to_string(decimalPart);
}
