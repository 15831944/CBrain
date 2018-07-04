#-------------------------------------------------
#
# Project created by QtCreator 2018-07-02T12:18:48
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CBrain
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialog_login.cpp \
    text_zeilenweise.cpp \
    users.cpp \
    dialog_user.cpp \
    umwandeln.cpp \
    dialog_settings_db.cpp \
    inifile.cpp

HEADERS  += mainwindow.h \
    defines_filenames.h \
    dialog_login.h \
    text_zeilenweise.h \
    users.h \
    dialog_user.h \
    umwandeln.h \
    dialog_settings_db.h \
    inifile.h

FORMS    += mainwindow.ui \
    dialog_login.ui \
    dialog_user.ui \
    dialog_settings_db.ui
