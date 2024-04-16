QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

VERSION = 2.3.2
QMAKE_TARGET_PRODUCT = Pomodoro
QMAKE_TARGET_DESCRIPTION = Simple minimalist pomodoro timer.
QMAKE_TARGET_COPYRIGHT = O.O. Paziura
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settings.cpp \
    settings_form.cpp

HEADERS += \
    mainwindow.h \
    settings.h \
    settings_form.h

TRANSLATIONS += \
    ../translations/Pomodoro_en.ts \
    ../translations/Pomodoro_ua.ts \
    ../translations/Pomodoro_ru.ts

RC_ICONS = Pomodoro.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
