#include "dialog_personal.h"
#include "ui_dialog_personal.h"

Dialog_personal::Dialog_personal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_personal)
{
    ui->setupUi(this);
    current_id = "0";
}

Dialog_personal::~Dialog_personal()
{
    delete ui;
}

void Dialog_personal::on_pushButton_ok_clicked()
{
    if( ui->lineEdit_vorname->text().isEmpty()  ||  ui->lineEdit_nachname->text().isEmpty()  )
    {
        QMessageBox mb;
        mb.setText("Die Datenfelder \"Vorname\" und \"Nachname\" duerfen nicht leer sein!");
        mb.exec();
    }else
    {
        text_zeilenweise tz;
        tz.zeile_anhaengen(ui->lineEdit_vorname->text());     //Wert 1
        tz.zeile_anhaengen(ui->lineEdit_nachname->text());    //Wert 2

        this->close();
        if(current_id == "0")
        {
            emit signal_send_data(tz);
        }else
        {
            emit signal_send_data(tz, current_id);
        }
    }
}

void Dialog_personal::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);
    emit signal_cancel();
}

void Dialog_personal::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_personal::set_data(text_zeilenweise daten, QString id)
{
    current_id = id;
    ui->lineEdit_vorname->setText(daten.zeile(1));
    ui->lineEdit_nachname->setText(daten.zeile(2));
}

void Dialog_personal::clear()
{
    ui->lineEdit_vorname->clear();
    ui->lineEdit_nachname->clear();
}
