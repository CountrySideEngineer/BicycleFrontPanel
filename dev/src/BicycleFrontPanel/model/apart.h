#ifndef APART_H
#define APART_H
#include <iostream>
#include <string>

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
    APart(uint8_t Pin,
          PART_PIN_DIRECTION PinDirection,
          uint32_t ChatteringTime,
          uint32_t PeridTime);

    virtual ~APart();

    virtual void Update(int32_t State) = 0;
    virtual void Update() = 0;
    virtual void InterruptCallback(int state);
    virtual void TimerCallback(int state);
    virtual bool CheckRecvData() { return false; }
    virtual void ResetRecvData() {}

public: //Getter/Setter

    int16_t GetState() { return this->mState; }
    uint8_t GetPin() { return  this->mPin; }
    void SetPin(uint8_t Pin) { this->mPin = Pin; }
    bool GetIsFailure() { return this->mIsFailure; }
    uint16_t GetFailureCode() { return this->mFailureCode; }
    void SetFailureCode(uint16_t FailureCode) { this->mFailureCode = FailureCode; }
    PART_PIN_DIRECTION GetPinDirection() { return this->mPinDirection; }
    void SetPinDirection(PART_PIN_DIRECTION PinDirection) { this->mPinDirection = PinDirection; }
    uint32_t GetChatteringTime() { return this->mChatteringTime; }
    uint32_t GetPeriodTime() { return this->mPeriodTime; }

    virtual uint8_t* GetBuffer() { return nullptr; }
    virtual uint GetBufferSize() { return 0; }
    virtual std::string ToString();

protected:
    int16_t     mState;
    uint8_t     mPin;
    uint32_t    mChatteringTime;
    uint32_t    mPeriodTime;
    bool        mIsFailure;
    uint16_t    mFailureCode;

    PART_PIN_DIRECTION mPinDirection;
};

#endif // APART_H
