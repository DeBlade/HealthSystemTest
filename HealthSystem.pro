#-------------------------------------------------
#
# Project created by QtCreator 2017-02-28T10:09:28
#
#-------------------------------------------------

QT       += core gui sql charts
QT += multimediawidgets
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HealthSystem
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        healthsystem.cpp \
    addnewuser.cpp \
    showuser.cpp \
    analysis.cpp \
    playvideo.cpp \
    openfilewidget.cpp \
    confirmvideopath.cpp \
    showchart.cpp \
    showreport.cpp \
    healthreaddata.cpp \
    opencamera.cpp \
    saggitalshowchart.cpp \
    frontalshowchart.cpp \
    welcomewidget.cpp \
    mainwindows.cpp

HEADERS  += healthsystem.h \
    addnewuser.h \
    showuser.h \
    analysis.h \
    playvideo.h \
    openfilewidget.h \
    confirmvideopath.h \
    showchart.h \
    showreport.h \
    healthreaddata.h \
    hdata.h \
    opencamera.h \
    saggitalshowchart.h \
    frontalshowchart.h \
    welcomewidget.h \
    mainwindows.h

FORMS    += healthsystem.ui \
    addnewuser.ui \
    showuser.ui \
    analysis.ui \
    playvideo.ui \
    openfilewidget.ui \
    confirmvideopath.ui \
    showchart.ui \
    showreport.ui \
    opencamera.ui \
    welcomewidget.ui \
    mainwindows.ui

RESOURCES += \
    health.qrc
