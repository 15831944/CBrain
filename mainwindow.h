#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include "brain.h"
#include "defines_filenames.h"
#include "dialog_login.h"
#include "users.h"

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

public slots:
    void slot_login(QString user, QString pwd);

private slots:
    void on_actionNetzwerkordner_aendern_triggered();

    void on_actionBenutzer_wechsen_triggered();

private:
    Ui::MainWindow *ui;
    bool isvalid;
    QString inifile;
    users u;
    brain cb;

    void clear();
    void read_inifile();
    void read_iniuser();
    void ui_rechte_admin();
    void ui_rechte_nobody();

};

#endif // MAINWINDOW_H
