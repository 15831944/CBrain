#include "dialog_lager_mengenkorrektur.h"
#include "ui_dialog_lager_mengenkorrektur.h"

Dialog_lager_mengenkorrektur::Dialog_lager_mengenkorrektur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_lager_mengenkorrektur)
{
    ui->setupUi(this);
    dbeigen = NULL;
}

Dialog_lager_mengenkorrektur::~Dialog_lager_mengenkorrektur()
{
    delete ui;
}

void Dialog_lager_mengenkorrektur::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_lager_mengenkorrektur::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Dialog_lager_mengenkorrektur::set_aid(QString artikelid)
{
    if(dbeigen != NULL)
    {
        ui->lineEdit_id->setText(artikelid);
        ui->lineEdit_nr->setText(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_NR, artikelid));
        ui->lineEdit_bez->setText(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZ, artikelid));
        ui->lineEdit_kom->setText(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_KOMENT, artikelid));
        QString me = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, artikelid);
        ui->lineEdit_bestand->setText(me);
        ui->spinBox_bestandneu->setValue(me.toInt());
    }
}

void Dialog_lager_mengenkorrektur::on_pushButton_ok_clicked()
{
    int me = ui->spinBox_bestandneu->value();
    this->close();
    emit signal_send_menge(int_to_qstring(me));
}
