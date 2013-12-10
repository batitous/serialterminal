#-------------------------------------------------
#
# Project created by QtCreator 2010-07-22T10:59:20
#
#-------------------------------------------------

QT += widgets

QMAKE += -j3

TEMPLATE = app

DESTDIR = ./

DEPENDPATH = .

INCLUDEPATH +=  . \
                ../../include \
                ../../source \
                ../../ui \
                ../../../babcode/babcode/include/
	
SOURCES += 	../../source/main.cpp\
                ../../source/Settings.cpp \
                ../../source/UtilsMacOsX.cpp \
                ../../source/SerialThread.cpp \
                ../../source/AboutUi.cpp \
                ../../source/MainWindow.cpp

HEADERS  += ../../include/Settings.h \
            ../../include/Utils.h \
            ../../include/SerialThread.h \
            ../../include/AboutUi.h \
            ../../include/MainWindow.h

FORMS    += ../../ui/mainwindow.ui \
            ../../ui/aboutui.ui

RESOURCES += ../../resources/resources.qrc

mac {
    OBJECTS_DIR = ./buildOSX/
    MOC_DIR = ./buildOSX/
    UI_DIR = ./buildOSX/

    TARGET = SerialTerminal

    LIBS += ../../../babcode/babcode/project/macosx/Build/Products/Release/libbabcode-macosx.a \
            -framework AppKit \
            -framework IOKit
}
