#-------------------------------------------------
#
# Project created by QtCreator 2018-10-15T23:58:11
#
#-------------------------------------------------

QT       += core gui qml quick quickwidgets positioning

#для dbservice
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = seye
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
    device.cpp \
    dialogadddevice.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    object.cpp \
    objectsmodel.cpp \
    polygon.cpp \
    polygonmodel.cpp \
    appengine.cpp \
    fakeconnector.cpp \
    iconnector.cpp \
    dbservice.cpp \
    delegate.cpp \
    passport.cpp \
    buttonzone.cpp \
    zoneinfo.cpp \
    mainwindow_admin.cpp

HEADERS += appengine.h \
    device.h \
    dialogadddevice.h \
    enums.h \
    fakeconnector.h \
    iconnector.h \
    login.h \
    mainwindow.h \
    object.h \
    objectsmodel.h \
    polygon.h \
    polygonmodel.h \
    dbservice.h \
    structs.h \
    delegate.h \
    passport.h \
    buttonzone.h \
    zoneinfo.h \
    mainwindow_admin.h

FORMS += \
    mainwindow.ui \
    dialogadddevice.ui \
    device.ui \
    login.ui\
    passport.ui \
    zoneinfo.ui \
    mainwindow2.ui \
    mainwindow2.ui

RESOURCES += \
    qml.qrc \
    icons.qrc \
    other.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    models/UseCase.qmodel \
    models/uml.qmodel
