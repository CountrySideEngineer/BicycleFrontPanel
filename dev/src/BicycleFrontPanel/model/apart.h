#ifndef APART_H
#define APART_H
#include <iostream>

/**
 * @brief The APart class   Abstract class of parts, to abstract parts of
 *                          bicycle and so on...
 */
class APart
{
public:
    enum PART_PIN_DIRECTION {
        PART_PIN_DIRECTION_INPUT = 0,
        PART_PIN_DIRECTION_OUTPUT,
        PART_PIN_DIRECTION_IN_OUT,
        PART_PIN_DIRECTION_MAX
    };
public:
    APart();
    APart(uint8_t GpioPin,
          PART_PIN_DIRECTION PinDirection,
          uint32_t ChatteringTime,
          uint32_t PeridTime);

    virtual ~APart();

    virtual void Update(int32_t State) = 0;
    virtual void Update() = 0;
    virtual void InterruptCallback(int state);
    virtual void TimerCallback(int state);

public: //Getter/Setter

    int16_t GetState() { return this->mState; }
    uint8_t GetGpioPin() { return this->mGpioPin; }
    void SetGpioPin(uint8_t GpioPin) { this->mGpioPin = GpioPin; }
    bool GetIsFailure() { return this->mIsFailure; }
    uint16_t GetFailureCode() { return this->mFailureCode; }
    void SetFailureCode(uint16_t FailureCode) { this->mFailureCode = FailureCode; }
    PART_PIN_DIRECTION GetPinDirection() { return this->mPinDirection; }
    void SetPinDirection(PART_PIN_DIRECTION PinDirection) { this->mPinDirection = PinDirection; }

protected:
    int16_t     mState;
    uint8_t     mGpioPin;
    uint32_t    mChatteringTime;
    uint32_t    mPeriodTime;
    bool        mIsFailure;
    uint16_t    mFailureCode;

    PART_PIN_DIRECTION mPinDirection;

};

#endif // APART_H
