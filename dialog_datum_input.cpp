#include "dialog_datum_input.h"
#include "ui_dialog_datum_input.h"

Dialog_datum_input::Dialog_datum_input(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_datum_input)
{
    ui->setupUi(this);
}

Dialog_datum_input::~Dialog_datum_input()
{
    delete ui;
}

void Dialog_datum_input::on_pushButton_cancel_clicked()
{
    this->close();
    emit signal_cancel();
}

void Dialog_datum_input::set_date(QDate datum)
{
    ui->calendarWidget->setSelectedDate(datum);
    ui->dateEdit->setDate(datum);
}

void Dialog_datum_input::on_dateEdit_userDateChanged(const QDate &date)
{
    ui->calendarWidget->setSelectedDate(date);
}

void Dialog_datum_input::on_calendarWidget_clicked(const QDate &date)
{
    ui->dateEdit->setDate(date);
}

void Dialog_datum_input::on_pushButton_ok_clicked()
{
    this->close();
    QDate da = ui->calendarWidget->selectedDate();
    emit signal_send_date(&da);
}
