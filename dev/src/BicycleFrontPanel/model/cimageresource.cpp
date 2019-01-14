#include <QString>
#include "cimageresource.h"

/**
 * @brief CImageResource::CImageResource    Default constructor.
 */
CImageResource::CImageResource()
    : mBrakeState(0)
    , mLightState(0)
{}

/**
 * @brief CImageResource::CImageResource    Constructor with state variables.
 * @param brakeStake    State of brake, on or off.
 * @param lightState    Light of state, off, on, or request.
 */
CImageResource::CImageResource(uint32_t brakeStake, uint32_t lightState)
    : mBrakeState(brakeStake)
    , mLightState(lightState)
{}

CImageResource::CImageResource(CBicycleState* bicycleState)
{
    CBicycleState::BICYCLE_STATE_BRAKE brakeState;
    CBicycleState::BICYCLE_STATE_LIGHT lightState;

    bicycleState->getState(&brakeState, &lightState);

    this->mBrakeState = static_cast<uint32_t>(brakeState);
    this->mLightState = static_cast<uint32_t>(lightState);
}


