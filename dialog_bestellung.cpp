#include "dialog_bestellung.h"
#include "ui_dialog_bestellung.h"

Dialog_bestellung::Dialog_bestellung(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_bestellung)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Dialog_bestellung::~Dialog_bestellung()
{
    delete ui;
}

void Dialog_bestellung::on_pushButton_cancel_clicked()
{
    this->close();
    emit signal_cancel();
}

void Dialog_bestellung::set_data(text_zeilenweise data)
{
    QString artikel_id, artikel_nr, artikel_bez, lieferant, ve, menge, liefertermin, kommentar;
    artikel_id      = data.zeile(1);
    artikel_nr      = data.zeile(2);
    artikel_bez     = data.zeile(3);
    lieferant       = data.zeile(4);
    ve              = data.zeile(5);
    menge           = data.zeile(6);
    liefertermin    = data.zeile(7);
    kommentar       = data.zeile(8);

    ui->lineEdit_artikel_id->setText(artikel_id);
    ui->lineEdit_artikel_nr->setText(artikel_nr);
    ui->lineEdit_artikel_bez->setText(artikel_bez);
    ui->lineEdit_lieferant->setText(lieferant);
    ui->lineEdit_ve->setText(ve);
    ui->spinBox_menge->setValue(menge.toInt());

    if(liefertermin == "0")
    {
        ui->radioButton_LT_unbekannt->setChecked(true);
        ui->radioButton_LT_bekannt->setChecked(false);
    }else
    {
        ui->radioButton_LT_unbekannt->setChecked(false);
        ui->radioButton_LT_bekannt->setChecked(true);
        //Datum zuweisen:

    }
    ui->lineEdit_kommentar->setText(kommentar);
}

void Dialog_bestellung::on_pushButton_ok_clicked()
{
    QString artikel_id, menge, liefertermin, kommentar;
    artikel_id = ui->lineEdit_artikel_id->text();
    menge = int_to_qstring(ui->spinBox_menge->value());
    if(ui->radioButton_LT_unbekannt->isChecked())
    {
        liefertermin = "0";
    }else
    {
        QDate datum = ui->calendarWidget_LT->selectedDate();
        int y,m,d;
        datum.getDate(&y, &m, &d);
        QString msg;
        msg += int_to_qstring(y);
        msg += "-";
        msg += int_to_qstring(m);
        msg += "-";
        msg += int_to_qstring(d);
        liefertermin = msg;
    }
    kommentar = ui->lineEdit_kommentar->text();
    text_zeilenweise data;
    data.zeile_anhaengen(artikel_id);
    data.zeile_anhaengen(menge);
    data.zeile_anhaengen(liefertermin);
    data.zeile_anhaengen(kommentar);
    this->close();
    emit signal_send_data(data);
}

void Dialog_bestellung::on_calendarWidget_LT_clicked()
{
    ui->radioButton_LT_unbekannt->setChecked(false);
    ui->radioButton_LT_bekannt->setChecked(true);
}
