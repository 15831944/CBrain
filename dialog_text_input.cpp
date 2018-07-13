#include "dialog_text_input.h"
#include "ui_dialog_text_input.h"

Dialog_text_input::Dialog_text_input(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_text_input)
{
    ui->setupUi(this);
}

Dialog_text_input::~Dialog_text_input()
{
    delete ui;
}

void Dialog_text_input::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_text_input::setup(QString new_window_title, QString new_labeltext)
{
    this->setWindowTitle(new_window_title);
    ui->label->setText(new_labeltext);
}

void Dialog_text_input::on_pushButton_ok_clicked()
{
    this->close();
    emit signal_userinput(ui->lineEdit->text());
}
