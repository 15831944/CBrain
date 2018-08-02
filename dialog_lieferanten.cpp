#include "dialog_lieferanten.h"
#include "ui_dialog_lieferanten.h"

Dialog_lieferanten::Dialog_lieferanten(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_lieferanten)
{
    ui->setupUi(this);
    current_id = "0";
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Dialog_lieferanten::~Dialog_lieferanten()
{
    delete ui;
}

void Dialog_lieferanten::on_pushButton_ok_clicked()
{
    if( ui->lineEdit_name->text().isEmpty() )
    {
        QMessageBox mb;
        mb.setText("Das Datenfeld \"Name\" darf nicht leer sein!");
        mb.exec();
    }else
    {
        text_zeilenweise tz;
        tz.zeile_anhaengen(ui->lineEdit_name->text().replace(" / ","/"));       //Wert 1
        tz.zeile_anhaengen(ui->lineEdit_komment->text());                       //Wert 2

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

void Dialog_lieferanten::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);    
}

void Dialog_lieferanten::on_pushButton_cancel_clicked()
{
    this->close();
    emit signal_cancel();
}

void Dialog_lieferanten::set_data(text_zeilenweise daten, QString id)
{
    current_id = id;
    ui->lineEdit_name->setText(daten.zeile(1));
    ui->lineEdit_komment->setText(daten.zeile(2));
}

void Dialog_lieferanten::clear()
{
    ui->lineEdit_name->clear();
}
