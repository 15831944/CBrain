#include "dialog_login.h"
#include "ui_dialog_login.h"

Dialog_login::Dialog_login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_login)
{
    ui->setupUi(this);
}

Dialog_login::~Dialog_login()
{
    delete ui;
}

void Dialog_login::on_pushButton_ok_clicked()
{
    this->close();
    emit signal_ok(ui->lineEdit_user->text(),   \
                   ui->lineEdit_pwd->text()     );
}

void Dialog_login::on_pushButton_cancel_clicked()
{
    this->close();    
}

void Dialog_login::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);
    emit signal_ok("",  \
                   ""   );
}
