#ifndef ABICYCLEPART_H
#define ABICYCLEPART_H

#include "model/apart.h"
#include "model/cbicycleitemmodel.h"


class ABicyclePart : public APart
{
public:
    ABicyclePart(CBicycleItemModel* model,
                 uint8_t Pin,
                 PART_PIN_DIRECTION PinDirection,
                 uint32_t ChatteringTime = 0,
                 uint32_t PeriodTime = 0);
    ABicyclePart(CBicycleItemModel* model,
                 uint8_t InputPin,
                 uint8_t OutputPin,
                 uint32_t ChatteringTime = 0,
                 uint32_t PeriodTime = 0);

    CBicycleItemModel* getModel() const { return this->mModel; }

protected:
    CBicycleItemModel* mModel;
};

#endif // ABICYCLEPART_H
