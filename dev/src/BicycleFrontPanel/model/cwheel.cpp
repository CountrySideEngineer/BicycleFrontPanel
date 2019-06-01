#include <iostream>
#include "model/cwheel.h"
using namespace std;

/**
 * @brief CWheel::CWheel    Default constructor.
 */
CWheel::CWheel()
    : APart(0, PART_PIN_DIRECTION_MAX, 0, 0)
    , mRpm(0)
    , mVelocity(0)
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
    , mVelocity(0)
{
    for (int index = 0; index < CWheel::RPM_BUFFER_SIZE; index++) {
        this->mRpmBuffer[index] = 0;
    }
    this->mRpmBufferIndex = 0;

    this->ResetRecvData();
}

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
    uint32_t rpmSum = 0;
    for (int index = 0; index < CWheel::RPM_BUFFER_SIZE; index++) {
        rpmSum += this->mRpmBuffer[index];
    }

    uint32_t rpmAverage = rpmSum / CWheel::RPM_BUFFER_SIZE;
    this->mRpm = (static_cast<uint16_t>(rpmAverage)  * 60) / mInterval;

    //Update index.
    this->mRpmBufferIndex++;
    if (CWheel::RPM_BUFFER_SIZE <= this->mRpmBufferIndex) {
        this->mRpmBufferIndex = 0;
    }

    /*
     * Reset values after update index because, if the values are reset
     * before the index, current buffer will be reset and no data has been
     * set the area.
     */
    this->mRpmBuffer[this->mRpmBufferIndex] = 0;
}

/**
 * @brief CWheel::Update    Update rotate count.
 */
void CWheel::Update()
{
    uint16_t velocityIntegerPart = 0;
    uint16_t velocityDecadePart = 0;

    printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
            this->mSpiBuffer[0],
            this->mSpiBuffer[1],
            this->mSpiBuffer[2],
            this->mSpiBuffer[3],
            this->mSpiBuffer[4],
            this->mSpiBuffer[5],
            this->mSpiBuffer[6]);

    this->mRpm = (uint16_t)(((uint16_t)this->mSpiBuffer[0])
            | ((uint16_t)this->mSpiBuffer[1] << 8));
    velocityDecadePart = (uint16_t)(((uint16_t)this->mSpiBuffer[2])
            | ((uint16_t)this->mSpiBuffer[3] << 8));
    velocityIntegerPart = (uint16_t)(((uint16_t)this->mSpiBuffer[4])
            | ((uint16_t)this->mSpiBuffer[5] << 8));
    //Convert accuracy of decade part from 0.001 into 0.01
    velocityDecadePart /= 10;

    this->mVelocity = (((uint32_t)velocityIntegerPart) * 100) + (uint32_t)velocityDecadePart;

    printf("RPM = %d, Velocity = %d\n", this->mRpm, this->mVelocity);
}

/**
 * @brief CWheel::Update    Update parameters.
 * @remarks Update nothing in this class.
 */
void CWheel::Update(int32_t /* state */) {}

/**
 * @brief CWheel::ToString  Return RPM value of string data.
 * @return  RPM value in string type.
 */
string CWheel::RpmToString()
{
    return to_string(this->mRpm);
}

/**
 * @brief CWheel::VelocityToString  Return velocity value of string data,
 *                                  with 0.01[km/h] LSB.
 * @return The velocity with 0.01[km/h] LSB.
 */
string CWheel::VelocityToString()
{
    uint32_t integerPart = this->mVelocity / 100;
    uint32_t decadePart = this->mVelocity % 100;

    return to_string(integerPart) + "." + to_string(decadePart);
}

/**
 * @brief CWheel::CheckRecvData Check consistency of receive, stored data in
 *                              receive data buffer.
 * @return True the data keep consistency, otherwise false.
 */
bool CWheel::CheckRecvData()
{
    uint8_t checkSum = 0;

    for (unsigned int bufferIndex = 0; bufferIndex < (this->mSpiBufferSize - 1); bufferIndex++) {
        checkSum = (uint8_t)((uint16_t)checkSum + (uint16_t)this->mSpiBuffer[bufferIndex]);
    }

    bool result = false;
    if (checkSum == this->mSpiBuffer[6]) {
        result = true;
    }

    return result;
}

void CWheel::ResetRecvData()
{
    for (uint32_t index = 0; index < this->mSpiBufferSize; index++) {
        this->mSpiBuffer[index] = 0;
    }
}
