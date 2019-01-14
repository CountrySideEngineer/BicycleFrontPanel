#include <assert.h>
#include "cbicyclestate.h"
#include "model/cgpio.h"
#include "model/cbrake.h"
#include "model/cwheel.h"
#include "model/clightauto.h"
#include "model/clightmanual.h"

//Defines GPIO pin number as macro.
#define GPIO_PIN_FRONT_BRAKE        (19)
#define GPIO_PIN_REAR_BRAKE         (26)
#define GPIO_PIN_WHEEL_ROTATION     (13)
#define GPIO_PIN_WHEEL_VELOCITY     (6)
#define GPIO_PIN_LIGHT              (5)

//Timer value
#define BRAKE_CHATTERING_TIME_MS    (20)    //20msec
#define LIGHT_PIN_SCAN_PERIOD       (100)   //100msec

CBicycleState::CBicycleState()
    : mFrontBrake(
          new CBrake(GPIO_PIN_FRONT_BRAKE, APart::PART_PIN_DIRECTION_INPUT,
                     BRAKE_CHATTERING_TIME_MS, 0))
    , mRearBrake(
          new CBrake(GPIO_PIN_REAR_BRAKE, APart::PART_PIN_DIRECTION_INPUT,
                     BRAKE_CHATTERING_TIME_MS, 0))
    , mWheel(
          new CBrake(GPIO_PIN_WHEEL_ROTATION, APart::PART_PIN_DIRECTION_INPUT,
                     0, 0))
    , mWheelVelocity(
          new CBrake(GPIO_PIN_WHEEL_VELOCITY, APart::PART_PIN_DIRECTION_INPUT,
                     0, 0))
    , mLight(
          new CLightAuto(GPIO_PIN_LIGHT, APart::PART_PIN_DIRECTION_INPUT,
                         0, LIGHT_PIN_SCAN_PERIOD))
    , mBrakeState(BICYCLE_STATE_BRAKE_OFF)
    , mLightState(BICYCLE_STATE_LIGHT_OFF)
{
    CGpio::Initialize();
    CGpio* instance = CGpio::GetInstance();

#define REGIST_ISR(GPIO_instance, part, edge)                       \
    do {                                                            \
        GPIO_instance->SetIsr(part->GetGpioPin(), edge, part);      \
    } while(0)

    REGIST_ISR(instance, this->mFrontBrake, 2);     //Both rising up and falling down.
    REGIST_ISR(instance, this->mRearBrake, 2);      //Both rising up and falling down.
    REGIST_ISR(instance, this->mWheel, 0);          //Rising up.
    REGIST_ISR(instance, this->mWheelVelocity, 0);  //Rising up.

#define REGIST_TIMER_ISR(GPIO_instance, part)                       \
    do {                                                            \
        GPIO_instance->SetTimeIsr(part);                            \
    } while(0)

    REGIST_TIMER_ISR(instance, this->mLight);
}

/**
 * @brief CBicycleState::Update Update state of brake, mBrakeState, and light, mLightState.
 */
void CBicycleState::Update()
{
    if (dynamic_cast<CBrake*>(this->mFrontBrake)->GetIsHold()) {
        if (dynamic_cast<CBrake*>(this->mRearBrake)->GetIsHold()) {
            this->mBrakeState = BICYCLE_STATE_BRAKE_BOTH;
        } else {
            this->mBrakeState = BICYCLE_STATE_BRAKE_FRONT;
        }
    } else {
        if (dynamic_cast<CBrake*>(this->mRearBrake)->GetIsHold()) {
            this->mBrakeState = BICYCLE_STATE_BRAKE_REAR;
        } else {
            this->mBrakeState = BICYCLE_STATE_BRAKE_OFF;
        }
    }

    ALight* light = dynamic_cast<ALight*>(this->mLight);
    BICYCLE_STATE_LIGHT LightStateTable
            [static_cast<int>(ALight::LIGHT_MODE::LIGHT_MODE_MAX)]
            [static_cast<int>(ALight::LIGHT_STATE::LIGHT_STATE_MAX)] = {
        { BICYCLE_STATE_LIGHT_OFF, BICYCLE_STATE_LIGHT_AUTO_ON, BICYCLE_STATE_LIGHT::BICYCLE_STATE_LIGHT_OFF },
        { BICYCLE_STATE_LIGHT_OFF, BICYCLE_STATE_LIGHT_MANUAL_ON, BICYCLE_STATE_LIGHT::BICYCLE_STATE_LIGHT_MANUAL_REQ }
    };
    ALight::LIGHT_STATE lightState = light->GetLightState();
    ALight::LIGHT_MODE lightMode = light->GetLightMode();
    this->mLightState = LightStateTable[lightMode][lightState];
}

/**
 * @brief CBicycleState::getState   Returns state of brake and light.
 * @param brakeState    Pointer to buffer to set state of brake.
 * @param lightState    Pointer to buffer to set state of light.
 */
void CBicycleState::getState(BICYCLE_STATE_BRAKE *brakeState, BICYCLE_STATE_LIGHT *lightState)
{
    assert(NULL != brakeState);
    assert(nullptr != brakeState);
    assert(NULL != lightState);
    assert(nullptr != lightState);

    *brakeState = this->mBrakeState;
    *lightState = this->mLightState;
}

/**
 * @brief CBicycleState::SwitchLightMode    Toggle list mode between AUTO and MANUAL.
 * @param mode  Mode to be new set. The value 0 means AUTO, 1 means MANUAL, otherwise no meaning.
 */
void CBicycleState::SwitchLightMode(int mode)
{
    ALight* newLight = nullptr;
    if (0 == mode) {
        newLight = new CLightAuto(this->mLight->GetGpioPin(),
                                  this->mLight->GetPinDirection(),
                                  this->mLight->GetChatteringTime(),
                                  this->mLight->GetPeriodTime());
    } else if (1 == mode) {
        newLight = new CLightManual(this->mLight->GetGpioPin(),
                                    this->mLight->GetPinDirection(),
                                    this->mLight->GetChatteringTime(),
                                    this->mLight->GetPeriodTime());
    } else {
        //Nothing to do.
    }

    if (nullptr != newLight) {
        CGpio* instance = CGpio::GetInstance();
        instance->RemoveTimeIsr(this->mLight);

        REGIST_TIMER_ISR(instance, newLight);
        this->mLight = newLight;
    }
}
