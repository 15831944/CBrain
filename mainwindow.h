#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define VERSIONSNUMMER  "1.2018.07.03"

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include "brain.h"
#include "defines_filenames.h"
#include "dialog_login.h"
#include "users.h"
#include "dialog_user.h"

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


public slots:
    void slot_login(QString user, QString pwd);
    void slot_get_users(users new_users);

private slots:
    void on_actionNetzwerkordner_aendern_triggered();
    void on_actionBenutzer_wechsen_triggered();
    void on_actionBenutzer_verwalten_triggered();

    void on_actionInfo_triggered();

private:
    Ui::MainWindow *ui;
    bool isvalid;
    text_zeilenweise inifile;
    users u;
    brain cb;

    void clear();
    void read_inifile();
    void write_inifile();
    void read_iniuser();
    void write_iniuser();
    void ui_rechte_admin();
    void ui_rechte_nobody();

};

#endif // MAINWINDOW_H
