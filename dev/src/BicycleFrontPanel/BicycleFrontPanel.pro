#-------------------------------------------------
#
# Project created by QtCreator 2018-09-01T17:11:39
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = BicycleFrontPanel
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        bicyclefrontmonitormainwindow.cpp \
    model/abicyclepart.cpp \
    model/cbicycleitemmodel.cpp \
    model/cbrakeitemmodel.cpp \
    model/cdatetime.cpp \
    model/cdatetimebuilder.cpp \
    model/cbrake.cpp \
    model/clight.cpp \
    model/cparts.cpp \
    model/cgpio.cpp \
    model/cwheel.cpp \
    model/cwheelitemmodel.cpp \
    model/cwheelvelocity.cpp \
    model/apart.cpp \
    model/alight.cpp \
    model/clightauto.cpp \
    model/clightmanual.cpp \
    model/cbicyclestate.cpp \
    model/cimageresourcemanager.cpp \
    model/cimageresource.cpp \
    view/cbicycleitemview.cpp \
    view/cbrakeitemview.cpp \
    view/cwheelitemview.cpp

HEADERS += \
        bicyclefrontmonitormainwindow.h \
    model/abicyclepart.h \
    model/cbicycleitemmodel.h \
    model/cbrakeitemmodel.h \
    model/cdatetime.h \
    model/cdatetimebuilder.h \
    model/cbrake.h \
    model/clight.h \
    model/cparts.h \
    model/cgpio.h \
    model/cwheel.h \
    model/cwheelitemmodel.h \
    model/cwheelvelocity.h \
    model/apart.h \
    model/alight.h \
    model/clightauto.h \
    model/clightmanual.h \
    model/cbicyclestate.h \
    model/cimageresourcemanager.h \
    model/cimageresource.h \
    model/gpio_pin_config.h \
    view/cbicycleitemview.h \
    view/cbrakeitemview.h \
    view/cwheelitemview.h

FORMS += \
        bicyclefrontmonitormainwindow.ui

INCLUDEPATH += \
    ./lib/include \
    ./model \
    ./view

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: linux:{
    target.path = /opt/$${TARGET}/bin
    LIBS += -lpthread \
        -lpigpio
#    target.path = /opt/$${TARGET}/bin
#    DEFINES += PC_BUILD_CONFIG
#    SOURCES += model/pigpio_dummy.c
}
else: linux-arm-gnueabi-g++:{
    target.path = /opt/$${TARGET}/bin
    LIBS += -lpthread \
        -lpigpio
}
else: unix:!android:{
    target.path = /opt/$${TARGET}/bin
    LIBS += -lpthread \
        -lpigpio
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
