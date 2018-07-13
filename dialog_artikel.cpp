#include "dialog_artikel.h"
#include "ui_dialog_artikel.h"

Dialog_artikel::Dialog_artikel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_artikel)
{
    ui->setupUi(this);
    current_id = "0";
}

Dialog_artikel::~Dialog_artikel()
{
    delete ui;
}

void Dialog_artikel::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_artikel::on_pushButton_ok_clicked()
{
    if( ui->lineEdit_artielnummer->text().isEmpty()     ||\
        ui->lineEdit_bezeichnung->text().isEmpty()      )
    {
        QMessageBox mb;
        mb.setText("Die Datenfelder \"Artikelnummer\" und \"Bezeichnung\" duerfen nicht leer sein!");
        mb.exec();
    }else
    {
        text_zeilenweise tz;
        tz.zeile_anhaengen(ui->lineEdit_artielnummer->text());      //Wert 1
        tz.zeile_anhaengen(ui->lineEdit_bezeichnung->text());       //Wert 2
        tz.zeile_anhaengen(ui->comboBox_lieferant->currentText());  //Wert 3

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

void Dialog_artikel::setup()
{
    //Unbedingt vorher Pointer auf DB zuweisen, sonst Programmabsturz!
    clear();
    text_zeilenweise lieferanten;
    //lieferanten aus Tabelle einlesen:
    if(dbeigen != NULL)
    {
        QSqlDatabase db;

        db = QSqlDatabase::database("dbglobal");
        db.setHostName(dbeigen->get_host());
        db.setDatabaseName(dbeigen->get_dbname());
        db.setUserName(dbeigen->get_user());
        db.setPassword(dbeigen->get_pwd());

        if(db.open())
        {
            QSqlQuery q(db);
            QString cmd;
            cmd += "SELECT ";
            cmd += PARAM_LIEFERANT_NAME;
            cmd += " FROM ";
            cmd += TABNAME_LIEFERANTEN;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    lieferanten.zeile_anhaengen(q.value(0).toString());
                }
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            db.close();

        }else
        {
            QMessageBox mb;
            mb.setText("Fehler bei Datenbankverbindung!");
            mb.exec();
        }
    }

    for(uint i=1; i<=lieferanten.zeilenanzahl() ;i++)
    {
        ui->comboBox_lieferant->addItem(lieferanten.zeile(i));
    }
}

void Dialog_artikel::set_data(text_zeilenweise daten, QString id)
{
    current_id = id;
    ui->lineEdit_artielnummer->setText(daten.zeile(1));
    ui->lineEdit_bezeichnung->setText(daten.zeile(2));
    ui->comboBox_lieferant->setCurrentIndex(ui->comboBox_lieferant->findText(daten.zeile(3)));
}

void Dialog_artikel::clear()
{
    ui->lineEdit_artielnummer->clear();
    ui->lineEdit_bezeichnung->clear();
    ui->comboBox_lieferant->clear();
}

void Dialog_artikel::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}
