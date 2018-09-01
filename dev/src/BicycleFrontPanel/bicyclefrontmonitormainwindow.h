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

protected slots:
    void onTimeout();

private:
    Ui::BicycleFrontMonitorMainWindow *ui;

    QTimer* mTimer;
    CDateTimeBuilder* mDateTimerBuilder;
};

#endif // BICYCLEFRONTMONITORMAINWINDOW_H
