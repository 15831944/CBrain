#include "dialog_artikel_beziehung.h"
#include "ui_dialog_artikel_beziehung.h"

Dialog_artikel_beziehung::Dialog_artikel_beziehung(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_artikel_beziehung)
{
    ui->setupUi(this);
    artikelid_eigen = "0";
    id_ist_const = false;
}

Dialog_artikel_beziehung::~Dialog_artikel_beziehung()
{
    delete ui;
}

void Dialog_artikel_beziehung::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Dialog_artikel_beziehung::on_pushButton_artikel_id_clicked()
{
    if(id_ist_const == true)
    {
        QMessageBox mb;
        mb.setText(tr("ID darf nicht verändert werden!"));
        mb.exec();

    }else
    {
        text_zeilenweise tz;
        text_zeilenweise ids;
        //-------------------------------------------
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
                cmd += "SELECT * FROM ";
                cmd += TABNAME_ARTIKEL;
                //------------------------
                cmd += " GROUP BY ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_ID;
                //------------------------
                cmd += " ORDER BY ";            //Sortiert nach:
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;

                if(q.exec(cmd))
                {
                    while(q.next())
                    {
                        ids.zeile_anhaengen(q.value(0).toString()); //ID

                        QString tmp;
                        tmp += q.value(1).toString();   //Artikelnummer
                        tmp += " ||| ";
                        tmp += q.value(2).toString();   //Bezeichnung
                        tz.zeile_anhaengen(tmp);
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
        //-------------------------------------------
        Dialog_dataselection *d = new Dialog_dataselection(this);

        for(uint i=1; i<=ids.zeilenanzahl() ;i++)
        {
            if(ids.zeile(i) == artikelid_eigen)
            {
                tz.zeile_loeschen(i);
                ids.zeile_loeschen(i);
                break;
            }
        }
        d->set_data(tz, ids);
        d->setWindowTitle("Artikel waelen (nur einen)");
        d->set_anz_returnwerte(1);

        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_set_artikel(text_zeilenweise))      );
        d->exec();
        delete d;
    }
}

void Dialog_artikel_beziehung::slot_set_artikel(text_zeilenweise artikel_ids)
{
    QString id = artikel_ids.zeile(1);

    if(id != artikelid_eigen)//eigentlich nicht nötig, weil vorher bereits raus gefiltert
    {
        ui->lineEdit_Artikel_id->setText(id);
        ui->lineEdit_Artikel_nr->setText(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_NR, id));
        ui->lineEdit_Artikel_bez->setText(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZ, id));
    }else
    {
        QMessageBox mb;
        mb.setText("Diese ID ist unzulaessig, weil Verweis auf Artikel selbst!");
        mb.exec();
    }

}

void Dialog_artikel_beziehung::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);
    emit signal_cancel();
}

void Dialog_artikel_beziehung::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_artikel_beziehung::on_pushButton_ok_clicked()
{
    if(!ui->lineEdit_Artikel_id->text().isEmpty())
    {
        text_zeilenweise data;

        QString artikel;
        artikel  =ui->lineEdit_Artikel_id->text();
        artikel += " | ";
        artikel += ui->lineEdit_Artikel_nr->text();
        artikel += " | ";
        artikel += ui->lineEdit_Artikel_bez->text();

        data.zeile_anhaengen(artikel);                                          //Wert 1 Artkelid(nr/bez)
        if(ui->checkBox_default->isChecked())
        {
            data.zeile_anhaengen("1");                                          //Wert 2
        }else
        {
            data.zeile_anhaengen("0");                                          //auch Wert 2
        }
        data.zeile_anhaengen(int_to_qstring(ui->spinBox_menge->value()));       //Wert 3

        this->close();
        emit signal_send_data(data);
    }else
    {
        QMessageBox mb;
        mb.setText("Artikel-ID darf nicht leer sein!");
        mb.exec();
    }
}

void Dialog_artikel_beziehung::set_data(text_zeilenweise data)
{
    text_zeilenweise tmpids;
    tmpids.set_text(data.zeile(1));//id
    slot_set_artikel(tmpids);

    if(data.zeile(2) == "1")//default
    {
        ui->checkBox_default->setChecked(true);
    }else
    {
        ui->checkBox_default->setChecked(false);
    }

    ui->spinBox_menge->setValue(data.zeile(3).toInt());//Menge
}

void Dialog_artikel_beziehung::set_id_const(bool iskonst)
{
    id_ist_const = iskonst;
}
