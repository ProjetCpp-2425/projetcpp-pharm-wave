

QT       += core gui sql printsupport widgets serialport
QT       += core gui  serialport
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
QT += network

QT += charts
QT += widgets charts sql
QT += core gui widgets printsupport


# Add the qrcodegen source file
INCLUDEPATH += $$PWD/QR-Code-generator-master/cpp
SOURCES += $$PWD/QR-Code-generator-master/cpp/qrcodegen.cpp \
    arduino.cpp

SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    patient.cpp \
    smtp.cpp

HEADERS += \
    arduino.h \
    connection.h \
    mainwindow.h \
    patient.h \
    smtp.h
    connection.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
