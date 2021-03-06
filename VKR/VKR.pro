#-------------------------------------------------
#
# Project created by QtCreator 2020-03-05T14:33:08
#
#-------------------------------------------------

QT       += core gui charts network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VKR1-0
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
        AUV/rov_model.cpp \
        AUV/su_rov.cpp \
        AUV/vehicle.cpp \
        Algorithms/a_star.cpp \
        Algorithms/d_lite.cpp \
        Algorithms/field_d.cpp \
        Algorithms/li.cpp \
        kx_pult/configdata.cpp \
        kx_pult/kx_protocol.cpp \
        kx_pult/qkx_coeffs.cpp \
        kx_pult/qpiconfig.cpp \
        main.cpp \
        mainwindow.cpp \
        map.cpp \
        obstacle.cpp

HEADERS += \
        AUV/rov_model.h \
        AUV/su_rov.h \
        AUV/vehicle.h \
        Algorithms/a_star.h \
        Algorithms/custom_priority_queue.h \
        Algorithms/d_lite.h \
        Algorithms/field_d.h \
        Algorithms/li.h \
        kx_pult/configdata.h \
        kx_pult/kx_protocol.h \
        kx_pult/qkx_coeffs.h \
        kx_pult/qpiconfig.h \
        mainwindow.h \
        map.h \
        obstacle.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
