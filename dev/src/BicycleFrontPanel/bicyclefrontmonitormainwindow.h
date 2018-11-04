#ifndef BICYCLEFRONTMONITORMAINWINDOW_H
#define BICYCLEFRONTMONITORMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "model/cparts.h"
#include "model/cbrake.h"
#include "model/cdatetimebuilder.h"

namespace Ui {
class BicycleFrontMonitorMainWindow;
}

class BicycleFrontMonitorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit BicycleFrontMonitorMainWindow(QWidget *parent = nullptr);
    ~BicycleFrontMonitorMainWindow();

    void updateDateTime();
    void updateLightState();
    void updateLightManualSw();

protected slots:
    void onTimeout();
    void onLightSw();
    void onLightAutoManSw();

private:
    Ui::BicycleFrontMonitorMainWindow *ui;

    bool mIsLightOn;
    bool mIsLightSwManual;
    bool mIsHoldFrontBrake;
    bool mIsHoldRearBrake;

    QTimer* mTimer;
    CDateTimeBuilder* mDateTimerBuilder;
    CParts* mFrontBrake;
    CParts* mRearBrake;
    CParts* mWheel;
    CParts* mWheelVelocity;
};

#endif // BICYCLEFRONTMONITORMAINWINDOW_H
