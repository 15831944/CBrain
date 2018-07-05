#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSIONSNUMMER  "1.2018.07.04"

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

#include "dialog_login.h"
#include "dialog_user.h"
#include "dialog_settings_db.h"

#include "form_tableeditor.h"

#include "defines_filenames.h"
#include "users.h"
#include "inifile.h"
#include "cbrainbatabase.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool setup();

signals:

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void slot_login(QString user, QString pwd);
    void slot_get_users(users new_users);
    void slot_get_settings_db_eigen(text_zeilenweise data);

private slots:
    void on_actionNetzwerkordner_aendern_triggered();
    void on_actionBenutzer_wechsen_triggered();
    void on_actionBenutzer_verwalten_triggered();
    void on_actionInfo_triggered();
    void on_actionProgrammeigene_Datenbank_triggered();
    void on_actionTestfunktion_triggered();
    void on_actionTabelleneditor_triggered();

private:
    Ui::MainWindow *ui;
    users u;
    QString user;
    inifile ini;

    //Datenbanken:
    cbrainbatabase dbeigen;

    //Flags:
    bool isvalid;               //Anwendungsumgebung korrekt (Inifiles + Ordner...)
    bool widget_tabedit_open;   //widget_tableeditor ist offen

    //eigene Widgets:
    Form_tableeditor widget_tableeditor;

    //Funktionen:
    void clear();
    void read_inifile();
    void write_inifile();
    void read_iniuser();
    void write_iniuser();
    void ui_rechte_admin();
    void ui_rechte_nobody();

};

#endif // MAINWINDOW_H
