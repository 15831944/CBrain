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
    emit signal_cancel();
}

void Dialog_artikel::on_pushButton_ok_clicked()
{
    if( ui->lineEdit_artielnummer->text().isEmpty()     ||\
        ui->lineEdit_bezeichnung->text().isEmpty()      )
    {
        QMessageBox mb;
        mb.setText("Die Datenfelder \"Artikelnummer\" und \"Bezeichnung\" duerfen nicht leer sein!");
        mb.exec();
    }else if( ui->comboBox_lieferant->currentText() == "---")
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst einen Lieferanten eintragen!");
        mb.exec();
    }else
    {
        text_zeilenweise tz;
        tz.zeile_anhaengen(ui->lineEdit_artielnummer->text());      //Wert 1
        tz.zeile_anhaengen(ui->lineEdit_bezeichnung->text());       //Wert 2
        QString lief = text_links(ui->comboBox_lieferant->currentText(), " / ");
        tz.zeile_anhaengen(lief);                                   //Wert 3
        tz.zeile_anhaengen(ui->lineEdit_lagerort->text());          //Wert 4
        tz.zeile_anhaengen(int_to_qstring(ui->spinBox_ve->value()));//Wert 5
        tz.zeile_anhaengen(ui->lineEdit_komment->text());           //Wert 6

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
    //text_zeilenweise lieferanten;
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
            cmd += PARAM_LIEFERANT_ID;
            cmd += ", ";
            cmd += PARAM_LIEFERANT_NAME;
            cmd += " FROM ";
            cmd += TABNAME_LIEFERANT;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    QString tmp;
                    tmp += q.value(0).toString();
                    tmp += " / ";
                    tmp += q.value(1).toString();
                    lieferanten.zeile_anhaengen(tmp);
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

    ui->comboBox_lieferant->addItem("---");
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

    QString tmp;
    tmp += daten.zeile(3);
    tmp += " / ";
    tmp += dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_NAME, daten.zeile(3));

    ui->comboBox_lieferant->setCurrentIndex(ui->comboBox_lieferant->findText(tmp));

    ui->lineEdit_lagerort->setText(daten.zeile(4));
    ui->spinBox_ve->setValue(daten.zeile(5).toInt());
    ui->lineEdit_komment->setText(daten.zeile(6));
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

void Dialog_artikel::on_lineEdit_filter_lieferant_textChanged(const QString &arg1)
{
    ui->comboBox_lieferant->clear();
    for(uint i=1; i<=lieferanten.zeilenanzahl() ;i++)
    {
        if(text_rechts(lieferanten.zeile(i), " / ").toUpper().contains(arg1.toUpper()))
        {
            ui->comboBox_lieferant->addItem(lieferanten.zeile(i));
        }
    }
}
