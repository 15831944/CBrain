#include "dialog_promatpos.h"
#include "ui_dialog_promatpos.h"

Dialog_promatpos::Dialog_promatpos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_promatpos)
{
    ui->setupUi(this);
    current_id = "0";
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Dialog_promatpos::~Dialog_promatpos()
{
    delete ui;
}

void Dialog_promatpos::on_pushButton_ok_clicked()
{
    if( ui->plainTextEdit_bez->toPlainText().isEmpty() )
    {
        QMessageBox mb;
        mb.setText("Das Datenfeld \"Positionsbezeichnung\" darf nicht leer sein!");
        mb.exec();
    }else
    {
        QString bez = ui->plainTextEdit_bez->toPlainText();
        bez.replace("\n", "#br#");
        text_zeilenweise tz;
        tz.zeile_anhaengen(bez);                                                //Wert 1
        tz.zeile_anhaengen(int_to_qstring(ui->spinBox_menge->value()));         //Wert 2

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

void Dialog_promatpos::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);
}

void Dialog_promatpos::on_pushButton_cancel_clicked()
{
    this->close();
    emit signal_cancel();
}

void Dialog_promatpos::set_data(text_zeilenweise daten, QString id)
{
    current_id = id;
    QString bez = daten.zeile(1);
    bez.replace("#br#", "\n");
    ui->plainTextEdit_bez->setPlainText(bez);
    ui->spinBox_menge->setValue(daten.zeile(2).toInt());
}
