#include <assert.h>
#include "cbicyclestate.h"
#include "model/cgpio.h"
#include "model/cbrake.h"
#include "model/cwheel.h"
#include "model/cwheelvelocity.h"
#include "model/clightauto.h"
#include "model/clightmanual.h"
using namespace std;

//Defines GPIO pin number as macro.
#define GPIO_PIN_FRONT_BRAKE        (19)
#define GPIO_PIN_REAR_BRAKE         (26)
#define GPIO_PIN_WHEEL_ROTATION     (13)
#define GPIO_PIN_WHEEL_VELOCITY     (6)
#define GPIO_PIN_LIGHT              (5)

#define PIN_WHEEL                   ((int)CGpio::CSpiMode::SPI_CE::SPI_CE_0)
#define PIN_FRONT_WHEEL             ((int)CGpio::CSpiMode::SPI_CE::SPI_CE_0)
#define PIN_REAR_WHEEL              ((int)CGpio::CSpiMode::SPI_CE::SPI_CE_1)
#define PIN_FRONT_BRAKE             (GPIO_PIN_FRONT_BRAKE)
#define PIN_REAR_BRAKE              (GPIO_PIN_REAR_BRAKE)
#define PIN_LIGHT                   (GPIO_PIN_LIGHT)

//Timer value
#define BRAKE_CHATTERING_TIME_MS    (20)    //20msec
#define LIGHT_PIN_SCAN_PERIOD       (100)   //100msec
#define ROTATE_VELOCITY_SCAN_PERIOD (1000)  //1000msec - 1 sec.

CBicycleState::CBicycleState()
    : mFrontBrake(
          new CBrake(GPIO_PIN_FRONT_BRAKE, APart::PART_PIN_DIRECTION_INPUT,
                     BRAKE_CHATTERING_TIME_MS, 0))
    , mRearBrake(
          new CBrake(GPIO_PIN_REAR_BRAKE, APart::PART_PIN_DIRECTION_INPUT,
                     BRAKE_CHATTERING_TIME_MS, 0))
    , mFrontWheel(
          new CWheel(PIN_WHEEL, APart::PART_PIN_DIRECTION_INPUT,
                     0, ROTATE_VELOCITY_SCAN_PERIOD))
    , mWheelVelocity(
          new CWheelVelocity(GPIO_PIN_WHEEL_VELOCITY, APart::PART_PIN_DIRECTION_INPUT,
                     0, ROTATE_VELOCITY_SCAN_PERIOD))
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
        GPIO_instance->SetIsr(part->GetPin(), edge, part);      \
    } while(0)

    REGIST_ISR(instance, this->mFrontBrake, 2);     //Both rising up and falling down.
    REGIST_ISR(instance, this->mRearBrake, 2);      //Both rising up and falling down.
    //REGIST_ISR(instance, this->mWheel, 0);          //Rising up.
    //REGIST_ISR(instance, this->mWheelVelocity, 0);  //Rising up.

#define REGIST_TIMER_ISR(GPIO_instance, part)                       \
    do {                                                            \
        GPIO_instance->SetTimeIsr(part);                            \
    } while(0)

    REGIST_TIMER_ISR(instance, this->mLight);
    //REGIST_TIMER_ISR(instance, this->mWheel);
    //REGIST_TIMER_ISR(instance, this->mWheelVelocity);

    CGpio::CSpiMode spiMode(CGpio::CSpiMode::SPI_CHANNEL::SPI_CHANNEL_MAIN,
                            CGpio::CSpiMode::SPI_MODE::SPI_MODE_0,
                            CGpio::CSpiMode::SPI_ACTIVE_MODE::SPI_ACTIVE_MODE_LOW,
                            CGpio::CSpiMode::SPI_ACTIVE_MODE::SPI_ACTIVE_MODE_LOW,
                            CGpio::CSpiMode::SPI_CLOCK::SPI_CLOCK_125K);
    instance->SetSPI(&spiMode);
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

    this->UpdateWheel();
}

#define SPI_COMMUNICATION_RETRY_MAX_COUNT   (4)
/**
 * @brief CBicycleState::UpdateWheel    Update value of CWheel object, rotate and velocity.
 */
void CBicycleState::UpdateWheel()
{
    this->UpdateWheel(this->mFrontWheel);
    this->UpdateWheel(this->mRearWheel);
}

/**
 * @brief   Update wheel data specified by argument.
 * @param   wheelPart   APart object to update.
 */
void CBicycleState::UpdateWheel(APart *wheelPart)
{
    try {
        this->ReadSPiData(wheelPart);

        wheelPart->Update();
    } catch (exception& ex) {
        printf("%s\r\n", ex.what());
        /*
         * @TODO:
         *  Operation to notify error to user.
         */
    }
}

/**
 * @brief   Read data via SPI to update APart object.
 * @param[out]  part    Pointer to APart object to updata.
 * @exception   invalid_argument    Thrown when the count of retry is over the limit.
 */
void CBicycleState::ReadSPiData(APart *part)
{
    CGpio* instance = CGpio::GetInstance();

    int loopCount = 0;
    do {
        instance->SpiRead((CGpio::CSpiMode::SPI_CE)part->GetPin(), part);
        loopCount++;
    } while ((false == part->CheckRecvData()) && (loopCount < SPI_COMMUNICATION_RETRY_MAX_COUNT));

    if (loopCount < SPI_COMMUNICATION_RETRY_MAX_COUNT) {
        throw invalid_argument("Receive data via SPI is incorrect.");
    }
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
        newLight = new CLightAuto(this->mLight->GetPin(),
                                  this->mLight->GetPinDirection(),
                                  this->mLight->GetChatteringTime(),
                                  this->mLight->GetPeriodTime());
    } else if (1 == mode) {
        newLight = new CLightManual(this->mLight->GetPin(),
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

/**
 * @brief CBicycleState::getRotate  Return rotate value of wheel.
 * @return  Rotate per minute value.
 */
uint32_t CBicycleState::getRotate()
{
    CWheel* wheel = static_cast<CWheel*>(this->mFrontWheel);

    return wheel->GetRpm();
}

/**
 * @brief CBicycleState::getVelocity    Return velocity, the speed, of wheel.
 * @return Velocity value.
 */
uint32_t CBicycleState::getVelocity()
{
    CWheel* wheel = static_cast<CWheel*>(this->mFrontWheel);

    return wheel->GetVelocity();
}

/**
 * @brief CBicycleState::GetRotateValue Returns RPM value in string data type.
 * @return  RPM value in string data type.
 */
string CBicycleState::GetRotateValue()
{
    auto wheelAuto = dynamic_cast<CWheel*>(this->mFrontWheel);
    return wheelAuto->RpmToString();
}

/**
 * @brief CBicycleState::GetVelocityValue   Returns velocity in string data type, its lsb
 *                                          is 0.1[m/h]
 * @return Velocity in string data type.
 */
string CBicycleState::GetVelocityValue()
{
    auto wheelAuto = dynamic_cast<CWheel*>(this->mFrontWheel);
    return wheelAuto->VelocityToString();
}
