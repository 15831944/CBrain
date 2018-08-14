#include "dialog_lager.h"
#include "ui_dialog_lager.h"

Dialog_lager::Dialog_lager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_lager)
{
    ui->setupUi(this);
    dbeigen = NULL;
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Dialog_lager::~Dialog_lager()
{
    delete ui;
}

void Dialog_lager::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Dialog_lager::setup()
{
    //Unbedingt vorher Pointer auf DB zuweisen, sonst Programmabsturz!
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
            cmd += PARAM_ARTIKEL_NR;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_LIEFERANT;
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    artikel_id.zeile_anhaengen(q.value(0).toString());
                    artikel_nr.zeile_anhaengen(q.value(1).toString());
                    artikel_bez.zeile_anhaengen(q.value(2).toString());
                    artikel_lie_id.zeile_anhaengen(q.value(3).toString());
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
                    projekt_id.zeile_anhaengen(q.value(0).toString());
                    projekt_name.zeile_anhaengen(q.value(1).toString());
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
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }
    artikel_id_gefiltert        = artikel_id;
    artikel_nr_gefiltert        = artikel_nr;
    artikel_bez_gefiltert       = artikel_bez;
    artikel_lie_id_gefiltert    = artikel_lie_id;//Lieferant
    projekt_id_gefiltert        = projekt_id;
    projekt_name_gefiltert      = projekt_name;

    for(uint i=1; i<=artikel_lie_id.zeilenanzahl() ;i++)
    {
        QString id = artikel_lie_id.zeile(i);
        bool vorhanden = false;
        for(uint ii=1; ii<=lie_id.zeilenanzahl() ;ii++)
        {
            if(lie_id.zeile(ii) == id)
            {
                vorhanden = true;
                break;
            }
        }
        if(vorhanden == false)
        {
            lie_id.zeile_anhaengen(id);
            QString name;
            name = dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_NAME, id);
            lie_name.zeile_anhaengen(name);
        }
    }
    ui->comboBox_lieferanten->addItem("ALLE");
    for(uint i=1; i<=lie_id.zeilenanzahl() ;i++)
    {
        QString zeile;
        zeile = lie_name.zeile(i);
        ui->comboBox_lieferanten->addItem(zeile);
    }

    ui->comboBox_artikel->addItem("---");
    for(uint i=1; i<=artikel_id.zeilenanzahl() ;i++)
    {
        QString zeile;
        zeile  = artikel_nr.zeile(i);
        zeile += " / ";
        zeile += artikel_bez.zeile(i);
        ui->comboBox_artikel->addItem(zeile);
    }
    ui->comboBox_kom->addItem("---");
    for(uint i=1; i<=projekt_id.zeilenanzahl() ;i++)
    {
        QString zeile;
        zeile = projekt_name.zeile(i);
        ui->comboBox_kom->addItem(zeile);
    }

}

void Dialog_lager::on_pushButton_ok_clicked()
{
    if(ui->comboBox_artikel->currentText() == "---")//nichts gewählt
    {
        QMessageBox mb;
        mb.setText(tr("Das Datenfeld \"Artikel\" darf nicht leer sein!"));
        mb.exec();
    }else if(ui->comboBox_kom->currentText() == "---" &&    \
             !ui->comboBox_kom->isHidden()                  )//nichts gewählt
    {
        QMessageBox mb;
        mb.setText(tr("Das Datenfeld \"Kommission\" darf nicht leer sein!"));
        mb.exec();
    }else
    {
        text_zeilenweise tz;
        tz.zeile_anhaengen(artikel_id_gefiltert.zeile(ui->comboBox_artikel->currentIndex() + 1));    //Wert 1
        tz.zeile_anhaengen(int_to_qstring(ui->spinBox_menge->value()));                              //Wert 2
        if(!ui->comboBox_kom->isHidden())
        {
            tz.zeile_anhaengen(projekt_id_gefiltert.zeile(ui->comboBox_kom->currentIndex() + 1));    //Wert 3
        }else
        {
            tz.zeile_anhaengen("no");        //Wert 3
        }
        tz.zeile_anhaengen(ui->lineEdit_komment->text());                                            //Wert 4
        if(!ui->lineEdit_lieferschein->isHidden())
        {
            tz.zeile_anhaengen(ui->lineEdit_lieferschein->text());                                   //Wert 5
        }else
        {
            tz.zeile_anhaengen("no");        //Wert 5
        }

        dlg_to_printmsg();
        emit signal_send_data(tz);
    }
}

void Dialog_lager::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);    
}

void Dialog_lager::on_pushButton_cancel_clicked()
{    
    this->close();
    printmsg.clear();
    emit signal_cancel();
}

void Dialog_lager::on_pushButton_print_clicked()
{
    Dialog_printbox *d = new Dialog_printbox;
    d->setText(printmsg);
    d->exec();
    delete d;
}

void Dialog_lager::dlg_to_printmsg()
{
    QString aid = artikel_id_gefiltert.zeile(ui->comboBox_artikel->currentIndex() + 1);
    QString anr = artikel_nr_gefiltert.zeile(ui->comboBox_artikel->currentIndex() + 1);
    QString abez = artikel_bez_gefiltert.zeile(ui->comboBox_artikel->currentIndex() + 1);
    QString lie = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LIEFERANT, aid,\
                                            TABNAME_LIEFERANT, PARAM_LIEFERANT_NAME);
    QString pro = projekt_name_gefiltert.zeile(ui->comboBox_kom->currentIndex() + 1);

    if(!printmsg.isEmpty())
    {
        printmsg += "\n";
        printmsg += "-----------------------------------------";
        printmsg += "\n";
    }
    printmsg += "Artikel: ";
    printmsg += anr;
    printmsg += " / ";
    printmsg += abez;
    printmsg += "\n";
    printmsg += "Lieferant: ";
    printmsg += lie;
    printmsg += "\n";
    if(vorgang.isEmpty())
    {
        printmsg += "Menge: ";
    }else
    {
        printmsg += vorgang;
        printmsg += ": ";
    }
    printmsg += int_to_qstring(ui->spinBox_menge->value());
    printmsg += " (VE=";
    printmsg += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_VE, aid);
    printmsg += ")";
    printmsg += "\n";
    printmsg += "Lagerort: ";
    printmsg += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERORT, aid);
    printmsg += "\n";
    printmsg += "Projekt: ";
    printmsg += pro;
    printmsg += "\n";
    printmsg += "Kommentar: ";
    printmsg += ui->lineEdit_komment->text();

}

void Dialog_lager::set_kommission_enabled(bool isit)
{
    if(isit == true)
    {
        ui->comboBox_kom->show();
        ui->label_kom->show();
        ui->lineEdit_komfilter->show();
        ui->label_komfilter->show();
    }else
    {
        ui->comboBox_kom->hide();
        ui->label_kom->hide();
        ui->lineEdit_komfilter->hide();
        ui->label_komfilter->hide();
    }
}

void Dialog_lager::set_lieferschein_enabled(bool isit)
{
    if(isit == true)
    {
        ui->lineEdit_lieferschein->show();
        ui->label_lieferschein->show();
    }else
    {
        ui->lineEdit_lieferschein->hide();
        ui->label_lieferschein->hide();
    }
}

//----------------------------------------Filter:
void Dialog_lager::on_lineEdit_artikelfilter_textChanged()
{
    artikel_id_gefiltert.clear();
    artikel_nr_gefiltert.clear();
    artikel_bez_gefiltert.clear();
    artikel_lie_id_gefiltert.clear();
    ui->comboBox_artikel->clear();

    for(uint i=1; i<=artikel_id.zeilenanzahl() ;i++)
    {
        QString suchbegriff = ui->lineEdit_artikelfilter->text().toUpper();
        QString anr = artikel_nr.zeile(i).toUpper();
        QString abez = artikel_bez.zeile(i).toUpper();

        if(ui->comboBox_lieferanten->currentText() == "ALLE")
        {
            if(anr.contains(suchbegriff)  ||  abez.contains(suchbegriff))
            {
                artikel_id_gefiltert.zeile_anhaengen(artikel_id.zeile(i));
                artikel_nr_gefiltert.zeile_anhaengen(artikel_nr.zeile(i));
                artikel_bez_gefiltert.zeile_anhaengen(artikel_bez.zeile(i));
                artikel_lie_id_gefiltert.zeile_anhaengen(artikel_lie_id.zeile(i));

                QString zeile;
                zeile  = artikel_nr.zeile(i);
                zeile += " / ";
                zeile += artikel_bez.zeile(i);
                ui->comboBox_artikel->addItem(zeile);
            }
        }else
        {
            QString lid = lie_id_gefiltert.zeile(ui->comboBox_lieferanten->currentIndex() + 1);
            if(artikel_lie_id.zeile(i) == lid)
            {
                if(anr.contains(suchbegriff)  ||  abez.contains(suchbegriff))
                {
                    artikel_id_gefiltert.zeile_anhaengen(artikel_id.zeile(i));
                    artikel_nr_gefiltert.zeile_anhaengen(artikel_nr.zeile(i));
                    artikel_bez_gefiltert.zeile_anhaengen(artikel_bez.zeile(i));
                    artikel_lie_id_gefiltert.zeile_anhaengen(artikel_lie_id.zeile(i));

                    QString zeile;
                    zeile  = artikel_nr.zeile(i);
                    zeile += " / ";
                    zeile += artikel_bez.zeile(i);
                    ui->comboBox_artikel->addItem(zeile);
                }
            }
        }
    }
}

void Dialog_lager::on_lineEdit_komfilter_textChanged(const QString &arg1)
{
    projekt_id_gefiltert.clear();
    projekt_name_gefiltert.clear();
    ui->comboBox_kom->clear();

    for(uint i=1; i<=projekt_name.zeilenanzahl() ;i++)
    {
        QString name = projekt_name.zeile(i);
        if(name.toUpper().contains(arg1.toUpper()))
        {
            projekt_id_gefiltert.zeile_anhaengen(projekt_id.zeile(i));
            projekt_name_gefiltert.zeile_anhaengen(name);
            ui->comboBox_kom->addItem(name);
        }
    }
}

void Dialog_lager::on_lineEdit_lieferantenfilter_textChanged(const QString &arg1)
{
    lie_id_gefiltert.clear();
    lie_name_gefiltert.clear();
    ui->comboBox_lieferanten->clear();

    if(arg1.isEmpty())
    {
        ui->comboBox_lieferanten->addItem("ALLE");
        for(uint i=1; i<=lie_id.zeilenanzahl() ;i++)
        {
            QString name = lie_name.zeile(i);
            lie_id_gefiltert.zeile_anhaengen(lie_id.zeile(i));
            lie_name_gefiltert.zeile_anhaengen(name);
            ui->comboBox_lieferanten->addItem(name);
        }
    }else
    {
        for(uint i=1; i<=lie_id.zeilenanzahl() ;i++)
        {
            QString name = lie_name.zeile(i);
            if(name.toUpper().contains(arg1.toUpper()))
            {
                lie_id_gefiltert.zeile_anhaengen(lie_id.zeile(i));
                lie_name_gefiltert.zeile_anhaengen(name);
                ui->comboBox_lieferanten->addItem(name);
            }
        }
    }

}

//----------------------------------------
