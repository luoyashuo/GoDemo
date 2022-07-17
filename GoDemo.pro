QT       += core gui   multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boardcontainorwindow.cpp \
    boardstate.cpp \
    dialog.cpp \
    goboardwindow.cpp \
    main.cpp \
    modelchoose.cpp \
    startwindow.cpp

HEADERS += \
    boardcontainorwindow.h \
    boardstate.h \
    dialog.h \
    goboardwindow.h \
    modelchoose.h \
    startwindow.h \
    test.h

FORMS += \
    boardcontainorwindow.ui \
    dialog.ui \
    goboardwindow.ui \
    modelchoose.ui \
    startwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc
