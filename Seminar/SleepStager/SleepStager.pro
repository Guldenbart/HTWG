#-------------------------------------------------
#
# Project created by QtCreator 2015-05-26T19:18:56
#
#-------------------------------------------------

QT       += core gui
CONFIG   += qwt
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SleepStager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    SleepStage.cpp

HEADERS  += mainwindow.h \
    SleepStage.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../dev/Aquila/aquila/lib/ -lAquila
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../dev/Aquila/aquila/lib/ -lAquila

INCLUDEPATH += $$PWD/../../../../dev/Aquila/aquila/include
DEPENDPATH += $$PWD/../../../../dev/Aquila/aquila/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../dev/Aquila/aquila/lib/ -lOoura_fft
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../dev/Aquila/aquila/lib/ -lOoura_fft

INCLUDEPATH += $$PWD/../../../../dev/Aquila/aquila/include
DEPENDPATH += $$PWD/../../../../dev/Aquila/aquila/include
