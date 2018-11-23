QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

INCLUDEPATH +=  ../../BicycleFrontPanel/ \
                ../../BicycleFrontPanel/model \

TEMPLATE = app

SOURCES +=  tst_cwheelvelocity_utest.cpp \
    ../../BicycleFrontPanel/model/apart.cpp \
    ../../BicycleFrontPanel/model/cwheel.cpp \
    ../../BicycleFrontPanel/model/cwheelvelocity.cpp
