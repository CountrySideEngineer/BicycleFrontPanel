#include "abicyclepart.h"

/**
 * @brief ABicyclePart::ABicyclePart    Constructor
 * @param model Model object
 * @param Pin   GPIO    GPIO pin number.
 * @param PinDirection  GPIO pin direction, input, output, or either.
 * @param ChatteringTime    Time to wait for finishing chattering.
 * @param PeriodTime        Periodic time to update.
 */
ABicyclePart::ABicyclePart(CBicycleItemModel* model,
                           uint8_t Pin,
                           PART_PIN_DIRECTION PinDirection,
                           uint32_t ChatteringTime,
                           uint32_t PeriodTime)
    : APart(Pin, PinDirection, ChatteringTime, PeriodTime)
    , mModel(model)
{}
