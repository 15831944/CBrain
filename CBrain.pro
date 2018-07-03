#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T12:18:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CBrain
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    brain.cpp \
    dialog_login.cpp \
    text_zeilenweise.cpp \
    users.cpp \
    dialog_user.cpp \
    umwandeln.cpp

HEADERS  += mainwindow.h \
    brain.h \
    defines_filenames.h \
    dialog_login.h \
    text_zeilenweise.h \
    users.h \
    dialog_user.h \
    umwandeln.h

FORMS    += mainwindow.ui \
    dialog_login.ui \
    dialog_user.ui
