#include "dialog_user.h"
#include "ui_dialog_user.h"

Dialog_user::Dialog_user(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_user)
{
    ui->setupUi(this);
}

Dialog_user::~Dialog_user()
{
    delete ui;
}

void Dialog_user::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_user::set_users(users u)
{
    user = u;
    ui->listWidget_names->clear();
    for(uint i=1; i<=u.get_names_tz().zeilenanzahl() ;i++)
    {
        ui->listWidget_names->addItem(u.get_names_tz().zeile(i));
    }
}

void Dialog_user::on_listWidget_names_currentRowChanged(int currentRow)
{
    ui->lineEdit_name->setText(user.get_names_tz().zeile(currentRow+1));
    ui->lineEdit_pwd->setText(user.get_pwd_tz().zeile(currentRow+1));

    if(user.get_isadmin_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_isadmin->setChecked(true);
    }else
    {
        ui->checkBox_isadmin->setChecked(false);
    }

    if(user.get_use_modul_artikel_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_modul_artikel->setChecked(true);
    }else
    {
        ui->checkBox_modul_artikel->setChecked(false);
    }

    if(user.get_use_modul_lieferanten_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_modul_lieferanten->setChecked(true);
    }else
    {
        ui->checkBox_modul_lieferanten->setChecked(false);
    }

    if(user.get_use_modul_lager_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_modul_lager->setChecked(true);
    }else
    {
        ui->checkBox_modul_lager->setChecked(false);
    }

    if(user.get_use_modul_projekte_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_modul_projekte->setChecked(true);
    }else
    {
        ui->checkBox_modul_projekte->setChecked(false);
    }

    ui->spinBox_id->setValue(user.get_id_tz().zeile(currentRow+1).toInt());

    if(user.get_use_modul_personal_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_modul_personal->setChecked(true);
    }else
    {
        ui->checkBox_modul_personal->setChecked(false);
    }

    if(user.get_use_modul_matlist_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_modul_matlist->setChecked(true);
    }else
    {
        ui->checkBox_modul_matlist->setChecked(false);
    }

    if(user.get_use_function_artfavsort_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_function_artfavsort->setChecked(true);
    }else
    {
        ui->checkBox_function_artfavsort->setChecked(false);
    }

    if(user.get_use_function_projektdel_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_function_projektdel->setChecked(true);
    }else
    {
        ui->checkBox_function_projektdel->setChecked(false);
    }

    if(user.get_use_modul_bestellungen_tz().zeile(currentRow+1)== "1")
    {
        ui->checkBox_modul_bestellungen->setChecked(true);
    }else
    {
        ui->checkBox_modul_bestellungen->setChecked(false);
    }

    //------------------------------------------------------------------------------------
    if(ui->listWidget_names->currentRow() == 0)//First Admin soll nicht editierbar sein!
    {
        ui->lineEdit_name->setDisabled(true);
        ui->lineEdit_pwd->setDisabled(true);
        ui->checkBox_isadmin->setDisabled(true);
        ui->checkBox_modul_artikel->setDisabled(true);
        ui->checkBox_modul_lieferanten->setDisabled(true);
        ui->checkBox_modul_lager->setDisabled(true);
        ui->checkBox_modul_projekte->setDisabled(true);
        ui->checkBox_modul_personal->setDisabled(true);
        ui->checkBox_modul_matlist->setDisabled(true);
        ui->checkBox_function_artfavsort->setDisabled(true);
        ui->checkBox_function_projektdel->setDisabled(true);
        ui->checkBox_modul_bestellungen->setDisabled(true);
    }else
    {
        ui->lineEdit_name->setEnabled(true);
        ui->lineEdit_pwd->setEnabled(true);
        ui->checkBox_isadmin->setEnabled(true);
        ui->checkBox_modul_artikel->setEnabled(true);
        ui->checkBox_modul_lieferanten->setEnabled(true);
        ui->checkBox_modul_lager->setEnabled(true);
        ui->checkBox_modul_projekte->setEnabled(true);
        ui->checkBox_modul_personal->setEnabled(true);
        ui->checkBox_modul_matlist->setEnabled(true);
        ui->checkBox_function_artfavsort->setEnabled(true);
        ui->checkBox_function_projektdel->setEnabled(true);
        ui->checkBox_modul_bestellungen->setEnabled(true);
    }
}

void Dialog_user::on_lineEdit_name_editingFinished()
{
    if(  user.change_name(ui->listWidget_names->currentRow()+1, ui->lineEdit_name->text())  )
    {
        ui->listWidget_names->currentItem()->setText(ui->lineEdit_name->text());
    }else
    {
        QString msg = "Der Name \"" + ui->lineEdit_name->text() + "\" ist bereits vergeben!";
        QMessageBox mb;
        mb.setText(msg.toStdString().c_str());
        mb.exec();
        ui->lineEdit_name->setText(ui->listWidget_names->currentItem()->text());
    }

}

void Dialog_user::on_lineEdit_pwd_editingFinished()
{
    user.change_pwd(ui->listWidget_names->currentRow()+1, ui->lineEdit_pwd->text());
}

void Dialog_user::on_checkBox_isadmin_toggled(bool checked)
{
    user.change_isadmin(ui->listWidget_names->currentRow()+1, checked);
}

void Dialog_user::on_pushButton_newuser_clicked()
{
    user.newuser();
    ui->listWidget_names->addItem(user.get_names_tz().zeile(user.get_names_tz().zeilenanzahl()));
    ui->listWidget_names->setCurrentRow(user.get_names_tz().zeilenanzahl()-1);
}

void Dialog_user::on_pushButton_deleteuser_clicked()
{
    if(user.is_admin(ui->listWidget_names->currentRow()+1))
    {
        QMessageBox mb;
        mb.setText(tr("Nutzer mit Admin-Rechten können nicht gelöscht werden!"));
        mb.exec();
    }else
    {
        user.removeuser(ui->listWidget_names->currentRow()+1);
        //Item aus QListWidget löschen:
        delete ui->listWidget_names->takeItem(ui->listWidget_names->currentRow());
    }
}

void Dialog_user::on_pushButton_ok_clicked()
{
    this->close();
    emit signal_send_users(user);
}

void Dialog_user::on_checkBox_modul_artikel_toggled(bool checked)
{
    user.change_use_modul_artikel(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_checkBox_modul_lieferanten_toggled(bool checked)
{
    user.change_use_modul_lieferanten(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_checkBox_modul_lager_toggled(bool checked)
{
    user.change_use_modul_lager(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_checkBox_modul_projekte_toggled(bool checked)
{
    user.change_use_modul_projekte(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_spinBox_id_valueChanged(const QString &arg1)
{
    user.change_id(ui->listWidget_names->currentRow()+1, arg1);
}
void Dialog_user::on_checkBox_modul_personal_toggled(bool checked)
{
    user.change_use_modul_personal(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_checkBox_modul_matlist_toggled(bool checked)
{
    user.change_use_modul_matlist(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_checkBox_function_artfavsort_toggled(bool checked)
{
    user.change_use_function_artfavsort(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_checkBox_function_projektdel_toggled(bool checked)
{
    user.change_use_function_projektdel(ui->listWidget_names->currentRow()+1, checked);
}
void Dialog_user::on_checkBox_modul_bestellungen_toggled(bool checked)
{
    user.change_use_modul_bestellungen(ui->listWidget_names->currentRow()+1, checked);
}
