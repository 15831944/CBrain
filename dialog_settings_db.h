#ifndef DIALOG_SETTINGS_DB_H
#define DIALOG_SETTINGS_DB_H

#include <QDialog>
#include <QtSql>
#include <QMessageBox>
#include "text_zeilenweise.h"

namespace Ui {
class Dialog_settings_db;
}

class Dialog_settings_db : public QDialog
{
    Q_OBJECT



signals:
    void signal_send_data(text_zeilenweise data);

public:
    explicit Dialog_settings_db(QWidget *parent = 0);
    ~Dialog_settings_db();
    void set_data(text_zeilenweise data);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_test_clicked();

private:
    Ui::Dialog_settings_db *ui;
    void clear();
    void setup();
};

#endif // DIALOG_SETTINGS_DB_H
