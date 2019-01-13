#ifndef CIMAGERESOURCE_H
#define CIMAGERESOURCE_H
#include <stdint.h>
#include "model/cbicyclestate.h"

class CImageResource
{
public:
    CImageResource();
    CImageResource(uint32_t brakeStake, uint32_t lightState);
    CImageResource(CBicycleState* bicycleState);
    ~CImageResource() {}

public://Getter/Setter
    uint32_t getBrakeState() { return this->mBrakeState; }
    uint32_t getLightState() { return this->mLightState; }

protected:
    uint32_t mBrakeState;
    uint32_t mLightState;
};

#endif // CIMAGERESOURCE_H
