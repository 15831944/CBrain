#include "dialog_lager.h"
#include "ui_dialog_lager.h"

Dialog_lager::Dialog_lager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_lager)
{
    ui->setupUi(this);
}

Dialog_lager::~Dialog_lager()
{
    delete ui;
}

void Dialog_lager::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Dialog_lager::clear()
{
    artikel.clear();
    projekte.clear();
    ui->comboBox_artikel->clear();
    ui->comboBox_kom->clear();
}

void Dialog_lager::setup()
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

        //-----------------------------------------------------
        if(db.open())
        {
            QSqlQuery q(db);
            QString cmd;

            //-----------------------------------------------------
            cmd += "SELECT ";
            cmd += PARAM_ARTIKEL_ID;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    QString tmp;
                    tmp += q.value(0).toString();
                    tmp += " / ";
                    tmp += q.value(1).toString();
                    artikel.zeile_anhaengen(tmp);
                }
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            //-----------------------------------------------------
            //-----------------------------------------------------
            cmd.clear();
            cmd += "SELECT ";
            cmd += PARAM_PROJEKT_ID;
            cmd += ", ";
            cmd += PARAM_PROJEKT_NAME;
            cmd += " FROM ";
            cmd += TABNAME_PROJEKT;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    QString tmp;
                    tmp += q.value(0).toString();
                    tmp += " / ";
                    tmp += q.value(1).toString();
                    projekte.zeile_anhaengen(tmp);
                }
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            //-----------------------------------------------------
            db.close();

        }else
        {
            QMessageBox mb;
            mb.setText("Fehler bei Datenbankverbindung!");
            mb.exec();
        }
    }

    ui->comboBox_artikel->addItem("---");
    for(uint i=1; i<=artikel.zeilenanzahl() ;i++)
    {
        ui->comboBox_artikel->addItem(artikel.zeile(i));
    }
    ui->comboBox_kom->addItem("---");
    for(uint i=1; i<=projekte.zeilenanzahl() ;i++)
    {
        ui->comboBox_kom->addItem(projekte.zeile(i));
    }

}

void Dialog_lager::on_pushButton_ok_clicked()
{
    if(ui->comboBox_artikel->currentText() == "---")//nichts gewählt
    {
        QMessageBox mb;
        mb.setText("Das Datenfeld \"Artikel\" darf nicht leer sein!");
        mb.exec();
    }else if(ui->comboBox_kom->currentText() == "---")//nichts gewählt
    {
        QMessageBox mb;
        mb.setText("Das Datenfeld \"Kommission\" darf nicht leer sein!");
        mb.exec();
    }else
    {
        text_zeilenweise tz;
        tz.zeile_anhaengen(text_links(ui->comboBox_artikel->currentText(), " / "));    //Wert 1
        tz.zeile_anhaengen(int_to_qstring(ui->spinBox_menge->value()));                //Wert 2
        tz.zeile_anhaengen(text_links(ui->comboBox_kom->currentText(), " / "));        //Wert 3
        tz.zeile_anhaengen(ui->lineEdit_komment->text());                              //Wert 4

        emit signal_send_data(tz);
    }
}

void Dialog_lager::on_pushButton_cancel_clicked()
{
    this->close();
}


void Dialog_lager::on_lineEdit_artikelfilter_textChanged(const QString &arg1)
{
    ui->comboBox_artikel->clear();
    for(uint i=1; i<=artikel.zeilenanzahl() ;i++)
    {
        if(text_rechts(artikel.zeile(i), " / ").toUpper().contains(arg1.toUpper()))
        {
            ui->comboBox_artikel->addItem(artikel.zeile(i));
        }
    }
}

void Dialog_lager::on_lineEdit_komfilter_textChanged(const QString &arg1)
{
    ui->comboBox_kom->clear();
    for(uint i=1; i<=projekte.zeilenanzahl() ;i++)
    {
        if(text_rechts(projekte.zeile(i), " / ").toUpper().contains(arg1.toUpper()))
        {
            ui->comboBox_kom->addItem(projekte.zeile(i));
        }
    }
}