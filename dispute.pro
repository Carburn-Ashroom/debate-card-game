QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    button.cpp \
    card.cpp \
    choose.cpp \
    heap.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_choose.cpp

HEADERS += \
    button.h \
    card.h \
    choose.h \
    config.h \
    heap.h \
    mainwindow.h \
    mainwindow_choose.h

FORMS += \
    mainwindow.ui \
    mainwindow_choose.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RC_FILE += dispute.rc
