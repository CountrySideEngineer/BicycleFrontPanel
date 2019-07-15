#include <iostream>
#include "model/cgpio.h"
#include "model/cwheel.h"
using namespace std;

/**
 * @brief CWheel::CWheel    Default constructor.
 */
CWheel::CWheel()
    : ABicyclePart(nullptr, 0, PART_PIN_DIRECTION_MAX, 0, 0)
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
    : ABicyclePart(nullptr, GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mRpm(0)
    , mVelocity(0)
{
    this->initBuffer();
    this->ResetRecvData();
}

CWheel::CWheel(CBicycleItemModel* model,
               uint8_t GpioPin,
               PART_PIN_DIRECTION PinDirection,
               uint32_t ChatteringTime,
               uint32_t PeriodTime)
    : ABicyclePart(model, GpioPin, PinDirection, ChatteringTime, PeriodTime)
    , mRpm(0)
    , mVelocity(0)
{
    this->initBuffer();
    this->ResetRecvData();
}

/**
 * @brief CWheel::initBuffer    Initialize Rpm buffer with argument initValue.
 * @param initValue Value to initialize buffer.
 */
void CWheel::initBuffer(uint32_t initValue)
{
    for (int index = 0; index < CWheel::RPM_BUFFER_SIZE; index++) {
        this->mRpmBuffer[index] = initValue;
    }
    this->mRpmBufferIndex = 0;
}

/**
 * @brief CWheel::InterruptCallback Callback function to be called when interrupt occurred.
 */
void CWheel::InterruptCallback(int /* state */)
{
    this->Update();
}

#define EXTRACT_BUFFER_DATA_UINT32(buffer, startIndex)              \
    static_cast<uint32_t>(                                          \
    (static_cast<uint32_t>(buffer[startIndex])) |                   \
    ((static_cast<uint32_t>(buffer[startIndex + 1])) << 8) |        \
    ((static_cast<uint32_t>(buffer[startIndex + 2])) << 16) |       \
    ((static_cast<uint32_t>(buffer[startIndex + 3])) << 24))

/**
 * @brief CWheel::TimerCallback Callback function to be called when the time dispatched.
 */
void CWheel::TimerCallback(int /* state */)
{
    this->initBuffer();

    CGpio* instance = CGpio::GetInstance();
    if (this->mSpiBufferSize ==
            static_cast<uint32_t>(instance->SpiRead(this->mPin, this->mSpiBuffer, this->mSpiBufferSize)))
    {
        if (this->CheckRecvData()) {
            uint32_t rotate = EXTRACT_BUFFER_DATA_UINT32(this->mSpiBuffer, 0);
            uint32_t integerPart = EXTRACT_BUFFER_DATA_UINT32(this->mSpiBuffer, 4);
            uint32_t decadePart = EXTRACT_BUFFER_DATA_UINT32(this->mSpiBuffer, 8);
            uint32_t velocity = integerPart * 1000 + decadePart;
            this->mModel->setData(this->mPin, rotate, velocity);
        } else {
            //Nothing ToDo.
        }
    }
}

/**
 * @brief CWheel::Update    Update rotate count.
 */
void CWheel::Update()
{
    uint16_t velocityIntegerPart = 0;
    uint16_t velocityDecadePart = 0;

#if 0
    printf("0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\n",
            this->mSpiBuffer[0],
            this->mSpiBuffer[1],
            this->mSpiBuffer[2],
            this->mSpiBuffer[3],
            this->mSpiBuffer[4],
            this->mSpiBuffer[5],
            this->mSpiBuffer[6]);
#endif

    this->mRpm = static_cast<uint16_t>(
                (static_cast<uint16_t>(this->mSpiBuffer[0])) |
                (static_cast<uint16_t>(this->mSpiBuffer[1]) << 8));
    velocityIntegerPart = static_cast<uint16_t>(
                (static_cast<uint16_t>(this->mSpiBuffer[2])) |
                (static_cast<uint16_t>(this->mSpiBuffer[3]) << 8));
    velocityDecadePart = static_cast<uint16_t>(
                (static_cast<uint16_t>(this->mSpiBuffer[4])) |
                (static_cast<uint16_t>(this->mSpiBuffer[5]) << 8));
    this->mVelocity = (static_cast<uint32_t>(velocityIntegerPart) * 1000)
            + static_cast<uint32_t>(velocityDecadePart);

#if 0
    printf("RPM = %d, Velocity = %d\n", this->mRpm, this->mVelocity);
#endif
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
        checkSum = static_cast<uint8_t>(
                    static_cast<uint16_t>(checkSum) +
                    static_cast<uint16_t>(this->mSpiBuffer[bufferIndex]));
    }

    uint checkSumIndex = this->mSpiBufferSize - 1;
    bool result = false;
    if (checkSum == this->mSpiBuffer[checkSumIndex]) {
        result = true;
    }

    return result;
}

/**
 * @brief CWheel::ResetRecvData Clear receive data buffer.
 */
void CWheel::ResetRecvData()
{
    for (uint32_t index = 0; index < this->mSpiBufferSize; index++) {
        this->mSpiBuffer[index] = 0;
    }
}

/**
 * @brief CWheel::Initialize    Initialize parameters with default data.
 */
void CWheel::Initialize()
{
    CBicycleItemModel* itemoModel = ABicyclePart::mModel;
    itemoModel->setData(this->mPin, 0, 0);
}
