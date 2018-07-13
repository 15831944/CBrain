#include "dialog_yes_no.h"
#include "ui_dialog_yes_no.h"

Dialog_yes_no::Dialog_yes_no(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_yes_no)
{
    ui->setupUi(this);
}

Dialog_yes_no::~Dialog_yes_no()
{
    delete ui;
}

void Dialog_yes_no::setup(QString msg)
{
    ui->plainTextEdit->setPlainText(msg);
}


void Dialog_yes_no::on_pushButton_yes_clicked()
{
    this->close();
    emit signal_yes();
}

void Dialog_yes_no::on_pushButton_no_clicked()
{
    this->close();
    emit signal_no();
}
