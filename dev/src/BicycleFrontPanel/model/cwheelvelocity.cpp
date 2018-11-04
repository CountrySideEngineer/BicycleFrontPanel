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
 * @brief CWheelVelocity::UpdateView
 */
void CWheelVelocity::UpdateView()
{
    if (nullptr == this->mDelegateWidget) { return; }

    qDebug() << "CWheelVelocity::UpdateView()"
             << this->mRotateCount << ","
             << this->mRpm << ","
             << this->mVelocity ;

    try {
        QString vHour = QString::number(this->mVelocity / 1000);
        QString vMin = QString::number((this->mVelocity / 100) % 10);
        QString velocity = vHour + "." + vMin;
        QLabel* label = dynamic_cast<QLabel*>(this->mDelegateWidget);
        label->setText(velocity);
    } catch (std::bad_cast& ex) {
        cout << ex.what() << endl;
    }

}


#define PI              (314)       //LSB = 0.01
#define INCH_PER_MM     (254)       //LSB = 0.1 mm
#define WHEEL_R_INCH    (26)        //LSB = 1.0
#define WHEEL_R_MM      (WHEEL_R_INCH * INCH_PER_MM)    //LSB = 0.01 * 0.1 = 1.0 * 10^(-3) mm

/**
 * @brief CWheelVelocity::Update    Calcurate velocty from RPM.
 */
void CWheelVelocity::Update()
{
    CWheel::Update();
    uint32_t velocity = 2 * PI * WHEEL_R_MM * this->mRpm;   //Velocity per minute.
    this->mVelocity = velocity * 60 / 1000;
}
