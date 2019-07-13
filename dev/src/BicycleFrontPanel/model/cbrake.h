#ifndef CBRAKE_H
#define CBRAKE_H
#include "model/abicyclepart.h"
#include "model/apart.h"

class CBrake : public ABicyclePart
{
public:
    CBrake();
    CBrake(uint8_t GpioPin,
           PART_PIN_DIRECTION PinDirection,
           uint32_t ChatteringTime = 0,
           uint32_t PeridTime = 0);
    CBrake(CBicycleItemModel* model,
           uint8_t GpioPin,
           PART_PIN_DIRECTION PinDirection,
           uint32_t ChatteringTime = 0,
           uint32_t PeridTime = 0);
    virtual ~CBrake() override;

public: //Getter/Setter
    void SetIsHold(bool isHold) { this->mIsHold = isHold; }
    bool GetIsHold() const { return this->mIsHold; }
    virtual void SetOptionPin(uint8_t optionPin) override;

public:
    virtual void Update(int32_t State)  override;
    virtual void Update() override;
    virtual void InterruptCallback(int state)  override;

    virtual void Initialize() override;

protected:
    bool mIsHold;
};

#endif // CBRAKE_H
