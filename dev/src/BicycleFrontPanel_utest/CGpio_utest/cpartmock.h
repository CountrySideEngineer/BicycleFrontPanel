#ifndef CPARTMOCK_H
#define CPARTMOCK_H
#include "model/apart.h"

class CPartMock : public APart
{
public:
    CPartMock();
    CPartMock(uint8_t GpioPin,
              PART_PIN_DIRECTION PinDirection,
              uint32_t ChatteringTime,
              uint32_t PeridTime);
    virtual ~CPartMock() {}

    virtual void Update(int32_t /* State */) {}
    virtual void Update() {}

    virtual void InterruptCallback(int state);

    int GetInterruptCallbackCalledCount() { return this->mInterruptCallbackCalledCount; }
    int GetInterruptCallback1StArg(int index) { return this->mInterruptCallback1StArg[index]; }

protected:
    int mInterruptCallbackCalledCount;
    int mInterruptCallback1StArg[1024];
};

#endif // CPARTMOCK_H
