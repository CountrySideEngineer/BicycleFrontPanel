#ifndef CWHEEL_H
#define CWHEEL_H
#include <iostream>
#include "model/abicyclepart.h"

/**
 * @brief The CWheel class  The class of wheel of bicycle.
 */
class CWheel : public ABicyclePart
{
public:
    explicit CWheel();
    explicit CWheel(uint8_t GpioPin,
                    PART_PIN_DIRECTION PinDirection,
                    uint32_t ChatteringTime,
                    uint32_t PeriodTime);
    explicit CWheel(CBicycleItemModel* model,
                    uint8_t GpioPin,
                    PART_PIN_DIRECTION PinDirection,
                    uint32_t ChatteringTime,
                    uint32_t PeriodTime);
    virtual ~CWheel() {}

    virtual void Update(int32_t state);
    virtual void Update();
    virtual void InterruptCallback(int state);
    virtual void TimerCallback(int state);

    virtual std::string RpmToString();
    virtual std::string VelocityToString();


public: //Getter/Setter
    uint32_t GetRpm() { return this->mRpm; }
    uint32_t GetVelocity() { return this->mVelocity; }

    virtual uint8_t* GetBuffer() { return this->mSpiBuffer; }
    virtual uint GetBufferSize() { return this->mSpiBufferSize; }

    virtual bool CheckRecvData();
    virtual void ResetRecvData();

protected:
    virtual void initBuffer(uint32_t initValue = 0);

protected:
    static const int RPM_BUFFER_SIZE = 5;

    uint32_t mRpm;
    uint32_t mVelocity;
    uint64_t mTmpCounter;

    uint32_t mRpmBuffer[RPM_BUFFER_SIZE];
    int mRpmBufferIndex;

#define SPI_BUFFER_SIZE (7)
    uint8_t mSpiBuffer[SPI_BUFFER_SIZE];
    const uint32_t mSpiBufferSize = SPI_BUFFER_SIZE;

    const uint16_t mInterval = 5000;    //Watch dog timer interval.
};

#endif // CWHEEL_H
