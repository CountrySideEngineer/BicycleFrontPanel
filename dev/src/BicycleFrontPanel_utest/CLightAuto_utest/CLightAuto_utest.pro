QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

INCLUDEPATH +=  ../../BicycleFrontPanel/ \
                ../../BicycleFrontPanel/model \

TEMPLATE = app

SOURCES += \ 
    tst_clightauto_utest.cpp \
    ../../BicycleFrontPanel/model/clightauto.cpp \
    ../../BicycleFrontPanel/model/alight.cpp \
    ../../BicycleFrontPanel/model/apart.cpp
