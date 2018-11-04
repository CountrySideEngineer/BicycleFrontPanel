#include <iostream>
#include <exception>
#include <typeinfo>
#include <QDebug>
#include <QLabel>
#include "cwheelvelocity.h"
using namespace std;

CWheelVelocity::CWheelVelocity() : CWheel () {}
CWheelVelocity::CWheelVelocity(uint Pin, QFrame* delegateWidget)
    : CWheel (Pin, delegateWidget)
    , mVelocity(0)
{}

/**
 * @brief CWheelVelocity::~CWheelVelocity   Destructor of CWheelVelocity.
 */
CWheelVelocity::~CWheelVelocity() {}

/**
 * @brief CWheelVelocity::UpdateView    Update view.
 */
void CWheelVelocity::UpdateView()
{
    if (nullptr == this->mDelegateWidget) { return; }

    qDebug() << "CWheelVelocity::UpdateView()"
             << this->mRotateCount << ","
             << this->mRpm << ","
             << this->mVelocity ;

    try {
        /*
         * The LSB of velocity variable, mVelocity, is 0.1 [m/h].
         * The LSB of velocity show in the display is 0.1 [km/h],
         * So the value unit is convert into one shown above.
         * The covnerte is run in 2 steps, calcurate km value, then meter one.
         * Now the physical value is calcurated in the formula below:
         *      PHY = CPU x LSB
         */
        /*
         * The variable mVelocity is divided by 10 to convert LSB from 0.1 [m/h]
         * into 1.0 [m/h].
         */
        QString velocityPhy_km = QString::number(this->mVelocity / 10 / 1000);
        /*
         * Formula in below calcurates the velocity after decimal point.
         */
        QString velocityPhy_m = QString::number(this->mVelocity % 10);
        /*
         * The value shown in the label has 0.1 [km/h] LSB.
         */
        QString velocity = velocityPhy_km + "." + velocityPhy_m;    //LSB : 0.1 [km/h]
        QLabel* label = dynamic_cast<QLabel*>(this->mDelegateWidget);
        label->setText(velocity);
    } catch (std::bad_cast& ex) {
        cout << ex.what() << endl;
    }
}

#define PI                  (314)       //LSB = 0.01
#define INCH_PER_MM         (254)       //LSB = 0.1 mm
#define WHEEL_R_INCH        (26)        //LSB = 1.0
#define WHEEL_R_MM      ((WHEEL_R_INCH) * (INCH_PER_MM))    //LSB = 0.01 * 0.1 = 1.0 * 10^(-3) mm
//#define WHEEL_CIRCUMFERENCE     WHEEL_R_MM
/*
 * The macro WHEEL_CIRCUMFERENCE shows circumference of wheel whose diameter
 * is 26 inch, about 2 meter.
 * The value is calcurated by 3.14 * 25.4 * 26 / 1000
 */
#define WHEEL_CIRCUMFERENCE (2073)     //LSB = 0.001 [m]

/**
 * @brief CWheelVelocity::Update    Calcurate velocty from RPM.
 */
void CWheelVelocity::Update()
{
    CWheel::Update();
    uint64_t velocity = WHEEL_CIRCUMFERENCE * this->mRpm;   //Velocity per minute.
    this->mVelocity = (velocity * 60) / 100;    //LSB = 0.1 [m/h]
}
