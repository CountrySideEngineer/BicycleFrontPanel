QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

INCLUDEPATH +=  ../../BicycleFrontPanel/ \
                ../../BicycleFrontPanel/model \

TEMPLATE = app

SOURCES +=  tst_cbrake_utest.cpp \
    ../../BicycleFrontPanel/model/cbrake.cpp \
    ../../BicycleFrontPanel/model/apart.cpp
