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

    void updateViews();
    void updateDateTime();
    void updateLightState();
    void updateLightManualSw();

protected slots:
    void onViewUpdateTimerTimeout();
    void onLightSw();
    void onLightAutoManSw();
    void on_menuButton_toggled(bool state);
    void on_lightConfigButton_toggled(bool state);

private:
    Ui::BicycleFrontMonitorMainWindow *ui;

    bool mIsLightOn;
    bool mIsLightSwManual;
    bool mIsHoldFrontBrake;
    bool mIsHoldRearBrake;

    QTimer* mViewUpdateTimer;
    CDateTimeBuilder* mDateTimerBuilder;
    CParts* mFrontBrake;
    CParts* mRearBrake;
    CParts* mWheel;
    CParts* mWheelVelocity;
};

#endif // BICYCLEFRONTMONITORMAINWINDOW_H
