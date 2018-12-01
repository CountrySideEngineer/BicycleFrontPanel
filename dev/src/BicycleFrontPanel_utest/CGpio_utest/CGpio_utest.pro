QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

INCLUDEPATH += \
    ../../BicycleFrontPanel/ \
    ../../BicycleFrontPanel/model \
    ../../BicycleFrontPanel/lib/include

SOURCES +=  tst_cgpio_utest.cpp \
    ../../BicycleFrontPanel/model/cgpio.cpp \
    pigpio_stub.cpp \
    ../../BicycleFrontPanel/model/cbrake.cpp \
    ../../BicycleFrontPanel/model/apart.cpp \
    cpartmock.cpp

HEADERS += \
    cpartmock.h
