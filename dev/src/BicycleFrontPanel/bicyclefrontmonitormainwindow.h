#ifndef BICYCLEFRONTMONITORMAINWINDOW_H
#define BICYCLEFRONTMONITORMAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "model/cparts.h"
#include "model/cbrake.h"
#include "model/cdatetimebuilder.h"
#include "model/cbicyclestate.h"
#include "model/cbrakeitemmodel.h"
#include "model/cwheelitemmodel.h"

namespace Ui {
class BicycleFrontMonitorMainWindow;
}

class BicycleFrontMonitorMainWindow : public QMainWindow
{
    Q_OBJECT

protected:
    enum {
        PAGE_INDEX_MAIN_PAGE = 0,
        PAGE_INDEX_MENU_PAGE,
        PAGE_INDEX_PAGE_NUM
    };

public:
    explicit BicycleFrontMonitorMainWindow(QWidget *parent = nullptr);
    ~BicycleFrontMonitorMainWindow();

    void updateViews();
    void updateDateTime();
    void updateLightState();
    void updateLightManualSw();

protected:
    void setup();
    void setupModelView();
    void setupDevices();
    void setupGpio();
    void initialize();

protected slots:
    void onViewUpdateTimerTimeout();
    void onParamUpdateTiemrTimeout();
    void onLightSw();
    void onLightAutoManSw();
    void on_menuButton_clicked(bool state);
    void on_appCloseButton_clicked(bool state);
    void on_moveMainPageButton_clicked(bool state);
    void on_lightConfigButton_clicked(bool state);


private:
    Ui::BicycleFrontMonitorMainWindow *ui;

    bool mIsLightOn;
    bool mIsLightSwManual;
    bool mIsHoldFrontBrake;
    bool mIsHoldRearBrake;

    QTimer* mViewUpdateTimer;
    QTimer* mParamUpdateTimer;
    CDateTimeBuilder* mDateTimerBuilder;
    APart* mFrontBrake;
    APart* mRearBrake;
    APart* mFrontWheel;
    APart* mRearWheel;
    APart* mLight;
    APart* mWheel;
    CBicycleState* mBicycleState;

    CBrakeItemModel* mBrakeItemModel;
    CWheelItemModel* mVelocityItemModel;
    CWheelItemModel* mRotateItemModel;
    CWheelItemModel* mWheelItemModel;

};

#endif // BICYCLEFRONTMONITORMAINWINDOW_H
