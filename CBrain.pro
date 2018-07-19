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
    inifile.cpp \
    cbrainbatabase.cpp \
    form_tableeditor.cpp \
    dialog_yes_no.cpp \
    dialog_text_input.cpp \
    dialog_tableparam.cpp \
    form_artikel.cpp \
    dialog_artikel.cpp \
    dialog_dataselection.cpp \
    text.cpp \
    datum.cpp \
    form_lieferanten.cpp \
    dialog_lieferanten.cpp \
    form_lager.cpp \
    form_projekte.cpp \
    dialog_projekte.cpp \
    form_backup.cpp \
    dialog_lager.cpp

HEADERS  += mainwindow.h \
    defines_filenames.h \
    dialog_login.h \
    text_zeilenweise.h \
    users.h \
    dialog_user.h \
    umwandeln.h \
    dialog_settings_db.h \
    inifile.h \
    cbrainbatabase.h \
    form_tableeditor.h \
    _todo.h \
    dialog_yes_no.h \
    dialog_text_input.h \
    dialog_tableparam.h \
    form_artikel.h \
    dialog_artikel.h \
    _tabellennamen.h \
    dialog_dataselection.h \
    text.h \
    datum.h \
    form_lieferanten.h \
    dialog_lieferanten.h \
    form_lager.h \
    form_projekte.h \
    dialog_projekte.h \
    form_backup.h \
    dialog_lager.h

FORMS    += mainwindow.ui \
    dialog_login.ui \
    dialog_user.ui \
    dialog_settings_db.ui \
    form_tableeditor.ui \
    dialog_yes_no.ui \
    dialog_text_input.ui \
    dialog_tableparam.ui \
    form_artikel.ui \
    dialog_artikel.ui \
    dialog_dataselection.ui \
    form_lieferanten.ui \
    dialog_lieferanten.ui \
    form_lager.ui \
    form_projekte.ui \
    dialog_projekte.ui \
    form_backup.ui \
    dialog_lager.ui
