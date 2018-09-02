#ifndef BICYCLEFRONTMONITORMAINWINDOW_H
#define BICYCLEFRONTMONITORMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
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
    void updateFrontBrakeState();
    void updateRearBrakeState();

protected slots:
    void onTimeout();
    void onLightSw();
    void onLightAutoManSw();

    //temporary event handler.
    void onFrontBrakeHold();
    void onFrontBrakeRelease();
    void onRearBrakeHold();
    void onRearBrakeRelease();

private:
    Ui::BicycleFrontMonitorMainWindow *ui;

    bool mIsLightOn;
    bool mIsLightSwManual;
    bool mIsHoldFrontBrake;
    bool mIsHoldRearBrake;

    QTimer* mTimer;
    CDateTimeBuilder* mDateTimerBuilder;
};

#endif // BICYCLEFRONTMONITORMAINWINDOW_H
