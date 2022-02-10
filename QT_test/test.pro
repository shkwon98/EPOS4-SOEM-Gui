QT       += core gui \
            network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32 {

INCLUDEPATH += $$PWD/../\
               C:\\opencv\\opencv_4_5_0\\build\\include\
               C:\\opencv\\opencv_4_5_0\\build\\include\\opencv2\
               #$$PWD/include/Intel_realsense/include\
               $$PWD/include/realsense/include\
               $$PWD/include/eigen-3.4.0\

CONFIG( debug, debug|release ) {
   LIBS +=$$PWD/lib/opencv/opencv_world450d.lib\   # debug
}
else {
   LIBS += $$PWD/lib/opencv/opencv_world450.lib\ # release
}


LIBS += $$PWD/lib/realsense/realsense2.lib\


DEFINES += _WINDOWS

}


SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
