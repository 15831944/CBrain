#ifndef DIALOG_USER_H
#define DIALOG_USER_H

#include <QDialog>
#include "users.h"
#include "umwandeln.h"

namespace Ui {
class Dialog_user;
}

class Dialog_user : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_user(QWidget *parent = 0);
    ~Dialog_user();

    void set_users(users u);

signals:
    void signal_send_users(users u);

public slots:


private slots:
    void on_pushButton_cancel_clicked();
    void on_listWidget_names_currentRowChanged(int currentRow);
    void on_lineEdit_name_editingFinished();
    void on_lineEdit_pwd_editingFinished();
    void on_checkBox_isadmin_toggled(bool checked);
    void on_pushButton_newuser_clicked();
    void on_pushButton_deleteuser_clicked();
    void on_pushButton_ok_clicked();
    void on_checkBox_modul_artikel_toggled(bool checked);
    void on_checkBox_modul_lieferanten_toggled(bool checked);
    void on_checkBox_modul_lager_toggled(bool checked);
    void on_checkBox_modul_projekte_toggled(bool checked);
    void on_spinBox_id_valueChanged(const QString &arg1);
    void on_checkBox_modul_personal_toggled(bool checked);

private:
    Ui::Dialog_user *ui;

    users user;
};

#endif // DIALOG_USER_H
