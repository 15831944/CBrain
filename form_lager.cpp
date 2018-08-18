#include "form_lager.h"
#include "ui_form_lager.h"

Form_lager::Form_lager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_lager)
{
    ui->setupUi(this);
    dbeigen = NULL;
    this->model = new QSqlQueryModel();
    this->model_artikel = new QSqlQueryModel();
}

Form_lager::~Form_lager()
{
    delete ui;
    delete model;
    delete model_artikel;
}

void Form_lager::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->geometry().height();
    int breite = this->geometry().width();
    int hbtn = ui->pushButton_in->height();
    int bbtn = breite/5;

    //Buttons Zeile 1:
    ui->pushButton_in->setFixedWidth(bbtn);
    ui->pushButton_in->move(1,\
                            1);
    ui->pushButton_out->setFixedWidth(bbtn);
    ui->pushButton_out->move(1 + bbtn + 1,\
                             1);

    //Buttons Zeile 2:
    ui->pushButton_inagain->setFixedWidth(bbtn);
    ui->pushButton_inagain->move(1,\
                                 1 + hbtn + 1);
    ui->pushButton_korrektur->setFixedWidth(bbtn);
    ui->pushButton_korrektur->move(1 + bbtn + 1,\
                                   1 + hbtn + 1);

    //links Tabelle Lager:
    ui->tableView->move(1,\
                        1 + (hbtn + 1)*2);
    ui->tableView->setFixedWidth( (breite-2)/3*2 );
    ui->tableView->setFixedHeight(hoehe - ui->tableView->pos().ry() -1);

    //rechts Suchleiste:
    ui->lineEdit_suche->setFixedHeight(ui->pushButton_in->geometry().height());
    ui->lineEdit_suche->setFixedWidth(breite/3);
    ui->lineEdit_suche->move(breite - 1 - ui->lineEdit_suche->geometry().width()\
                             ,1);

    ui->label_suche->setFixedHeight(ui->pushButton_in->geometry().height());
    ui->label_suche->move(breite - 1 -ui->lineEdit_suche->geometry().width() - 1\
                          - ui->label_suche->geometry().width()\
                          ,1);    

    //rechts Tabelle Artikel:
    ui->tableView_artikel->move(1 + ui->tableView->geometry().width() + 1,\
                                1 + ui->pushButton_in->geometry().height() + 1);
    ui->tableView_artikel->setFixedWidth( (breite-2)/3 );
    ui->tableView_artikel->setFixedHeight(ui->tableView->geometry().height());

    QWidget::resizeEvent(event);
}

void Form_lager::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_lager::set_user(QString u)
{
    user = u;
}

void Form_lager::show()
{
    update_table();
    setVisible(true);
}

void Form_lager::update_table()
{
    //-------------------------------------------
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
            //-------------------------------------------
            cmd += "SELECT ";
            //------------------------
            //cmd += TABNAME_VORGANG;
            cmd += "V";
            cmd += ".";
            cmd += PARAM_VORGANG_KLARTEXT;
            cmd += " AS ";
            cmd += "Vorgang";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += " AS ";
            cmd += "Artikelbez";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_NR;
            cmd += " AS ";
            cmd += "Artikelnr";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_MENGE;
            cmd += " AS ";
            cmd += "Menge";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_NAME;
            cmd += " AS ";
            cmd += "Kommission";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_LIEFERSCHEIN;
            cmd += " AS ";
            cmd += "Lieferschein";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_KOMMENT;
            cmd += " AS ";
            cmd += "Kommentar";
            cmd += ", ";
            //------------------------
            //cmd += TABNAME_PERSONAL;
            cmd += "E";
            cmd += ".";
            cmd += PARAM_PERSONAL_NACHNAME;
            cmd += " AS ";
            cmd += "Ersteller";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_DATERST;
            cmd += " AS ";
            cmd += "Erfassung";
            //cmd += ", ";
            //------------------------
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_LAGER;
            //------------------------
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_ARTIKEL;
            cmd += " ON ";
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_ARTIKELID;
            cmd += " = ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            //------------------------
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_PROJEKT;
            cmd += " ON ";
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_KOMMISSION;
            cmd += " = ";
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_ID;
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_PERSONAL;
            cmd += " AS ";
            cmd += "E";
            cmd += " ON (";
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_ERSTELLER;
            cmd += " = ";
            //cmd += TABNAME_PERSONAL;
            cmd += "E";
            cmd += ".";
            cmd += PARAM_PERSONAL_ID;
            cmd += ")";
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_VORGANG;
            cmd += " AS ";
            cmd += "V";
            cmd += " ON (";
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_VORGANG;
            cmd += " = ";
            //cmd += TABNAME_PERSONAL;
            cmd += "V";
            cmd += ".";
            cmd += PARAM_VORGANG_ID;
            cmd += ")";
            //------------------------
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_PROJEKT;
                cmd += ".";
                cmd += PARAM_PROJEKT_NAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_LAGER;
                cmd += ".";
                cmd += PARAM_LAGER_LIEFERSCHEIN;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_ID;
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_ID;
            cmd += " DESC";
            //------------------------

            if(q.exec(cmd))
            {
                model->setQuery(q);
                ui->tableView->setModel(model);

                QString msg;
                msg += int_to_qstring(model->rowCount());
                msg += " Buchungen:";
                ui->label_suche->setText(msg);

            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            //-------------------------------------------
            //-------------------------------------------
            cmd.clear();
            cmd += "SELECT ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            cmd += " AS ";
            cmd += "ID";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_NR;
            cmd += " AS ";
            cmd += "Artikelnummer";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += " AS ";
            cmd += "Artikelbez";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LAGERSTAND;
            cmd += " AS ";
            cmd += "Lagerstand";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTELLT;
            cmd += " AS ";
            cmd += "Bestellt";
            //cmd += ", ";
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;
            //------------------------
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_NR;
            //------------------------

            if(q.exec(cmd))
            {
                model_artikel->setQuery(q);
                ui->tableView_artikel->setModel(model_artikel);
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            //-------------------------------------------
            //-------------------------------------------
            db.close();

        }else
        {
            QMessageBox mb;
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }
    //-------------------------------------------
}

void Form_lager::on_lineEdit_suche_textChanged()
{
    update_table();
}

//------------------------------------Buttons:
void Form_lager::on_pushButton_in_clicked()
{
    Dialog_lager *d = new Dialog_lager(this);
    d->set_db(dbeigen);
    d->setup();
    d->setWindowTitle("Waren-Eingang buchen");
    d->set_vorgang("Waren-Eingang");
    d->set_kommission_enabled(false);
    d->set_lieferschein_enabled(true);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_in(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_lager::on_pushButton_out_clicked()
{
    Dialog_lager *d = new Dialog_lager(this);
    d->set_db(dbeigen);
    d->setup();
    d->setWindowTitle("Waren-Ausgang buchen");
    d->set_vorgang("Waren-Ausgang");
    d->set_kommission_enabled(true);
    d->set_lieferschein_enabled(false);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_out(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_lager::on_pushButton_inagain_clicked()
{
    Dialog_lager *d = new Dialog_lager(this);
    d->set_db(dbeigen);
    d->setup();
    d->setWindowTitle("Wieder-Einlagerung buchen");
    d->set_vorgang("Wieder-Einlagerung");
    d->set_kommission_enabled(true);
    d->set_lieferschein_enabled(false);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_inagain(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_lager::on_pushButton_korrektur_clicked()
{
    Dialog_text_input *d = new Dialog_text_input(this);
    d->setWindowTitle("Bestandskorrektur");
    d->set_infotext("Bitte Artikel-ID eingeben");
    connect(d, SIGNAL(signal_userinput(QString)),   \
            this, SLOT(slot_bestkor_aid(QString))   );
    d->exec();
    delete d;
}

//------------------------------------slots:
void Form_lager::slot_in(text_zeilenweise data)
{
    text_zeilenweise param, values;

    QString artikelid   = data.zeile(1);
    QString menge       = data.zeile(2);
    QString projektid   = data.zeile(3);
    QString kommentar   = data.zeile(4);
    QString lieschein   = data.zeile(5);    //Lieferschein

    param.zeile_anhaengen(PARAM_LAGER_VORGANG);
    param.zeile_anhaengen(PARAM_LAGER_ARTIKELID);
    param.zeile_anhaengen(PARAM_LAGER_MENGE);
    param.zeile_anhaengen(PARAM_LAGER_ERSTELLER);
    param.zeile_anhaengen(PARAM_LAGER_DATERST);
    if(projektid != "no")
    {
        param.zeile_anhaengen(PARAM_LAGER_KOMMISSION);
    }
    param.zeile_anhaengen(PARAM_LAGER_KOMMENT);
    param.zeile_anhaengen(PARAM_LAGER_LIEFERSCHEIN);

    values.zeile_anhaengen(VORGANG_WARENEINGANG);
    values.zeile_anhaengen(artikelid);
    values.zeile_anhaengen(menge);
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d_h_m_s());
    if(projektid != "no")
    {
        values.zeile_anhaengen(projektid);
    }
    values.zeile_anhaengen(kommentar);
    values.zeile_anhaengen(lieschein);

    //Aktuelle Lagermenge des Artikeln abfragen und ändern:
    QString menge_vorher = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, artikelid);
    int menge_akt = menge_vorher.toInt() + menge.toInt();
    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, int_to_qstring(menge_akt), artikelid);

    //Bestellte Menge des Artikeln abfragen und ändern:
    int in_bestellung = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, artikelid).toInt();
    if(in_bestellung < menge.toInt())
    {
        QMessageBox mb;
        mb.setText(tr("Achtung!\nBestellte Menge < gelieferte Menge!\n"));
        mb.exec();
        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, "0", artikelid);
    }else
    {
        menge_akt = in_bestellung - menge.toInt();
        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, int_to_qstring(menge_akt), artikelid);
    }


    //Menge in Bestellung als geliefert austragen:
    text_zeilenweise best_ids, best_bestellt, best_geliefert;
    //------------------------------------------------
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
            cmd += PARAM_BESTELLUNG_ID;
            cmd += ", ";
            cmd += PARAM_BESTELLUNG_ME_BEST;
            cmd += ", ";
            cmd += PARAM_BESTELLUNG_ME_GELIEFERT;
            cmd += " FROM ";
            cmd += TABNAME_BESTELLUNG;
            cmd += " WHERE ";
            cmd += "(";
            cmd += PARAM_BESTELLUNG_ARTIKELID;
            cmd += " = ";
            cmd += artikelid;
            cmd += ")";
            cmd += " AND ";
            cmd += "(";
            cmd += PARAM_BESTELLUNG_ME_BEST;
            cmd += " != ";
            cmd += PARAM_BESTELLUNG_ME_GELIEFERT;
            cmd += ")";

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    best_ids.zeile_anhaengen(q.value(0).toString());
                    best_bestellt.zeile_anhaengen(q.value(1).toString());
                    best_geliefert.zeile_anhaengen(q.value(2).toString());
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
    //------------------------------------------------
    int restmenge = menge.toInt();
    for(uint i=1; i<=best_ids.zeilenanzahl() ;i++)
    {
        int dif = best_bestellt.zeile(i).toInt() - best_geliefert.zeile(i).toInt();
        if(dif >= restmenge)
        {
            menge_akt = best_geliefert.zeile(i).toInt() + restmenge;
            dbeigen->data_edit(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ME_GELIEFERT, \
                               int_to_qstring(menge_akt), best_ids.zeile(i));
            restmenge = 0;
        }else
        {
            dbeigen->data_edit(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ME_GELIEFERT, \
                               best_bestellt.zeile(i), best_ids.zeile(i));
            restmenge = restmenge - dif;
        }
        if(restmenge == 0)
        {
            break;
        }
    }
    if(restmenge > 0)//Wenn mehr geliefert als insgesamt bestellt wurde
    {
        QString id = best_ids.zeile(best_ids.zeilenanzahl());
        menge_akt =dbeigen->get_data_qstring(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ME_GELIEFERT, id).toInt();
        menge_akt += restmenge;
        dbeigen->data_edit(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ME_GELIEFERT, \
                           int_to_qstring(menge_akt), id);
    }
    //------------------------------------------------
    dbeigen->data_new(TABNAME_LAGER, param, values);
    update_table();

    QMessageBox mb;
    mb.setText(tr("Buchung erfolgreich durchgeführt."));
    mb.exec();
}

void Form_lager::slot_out(text_zeilenweise data)
{
    text_zeilenweise param, values;

    QString artikelid   = data.zeile(1);
    QString menge       = data.zeile(2);
    QString projektid   = data.zeile(3);
    QString kommentar   = data.zeile(4);

    param.zeile_anhaengen(PARAM_LAGER_VORGANG);
    param.zeile_anhaengen(PARAM_LAGER_ARTIKELID);
    param.zeile_anhaengen(PARAM_LAGER_MENGE);
    param.zeile_anhaengen(PARAM_LAGER_ERSTELLER);
    param.zeile_anhaengen(PARAM_LAGER_DATERST);
    param.zeile_anhaengen(PARAM_LAGER_KOMMISSION);
    param.zeile_anhaengen(PARAM_LAGER_KOMMENT);

    values.zeile_anhaengen(VORGANG_WARENAUSGANG);
    values.zeile_anhaengen(artikelid);
    values.zeile_anhaengen(menge);
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d());
    values.zeile_anhaengen(projektid);
    values.zeile_anhaengen(kommentar);

    //Aktuelle Lagermenge des Artikeln abfragen:
    QString menge_vorher = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, artikelid);
    int menge_akt = menge_vorher.toInt() - menge.toInt();

    if(menge_akt >= 0)
    {
        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, int_to_qstring(menge_akt), artikelid);

        dbeigen->data_new(TABNAME_LAGER, param, values);
        update_table();

        //heraus bekommen, ob zu diesem Artikel bereits ein Eintrag in der promat_* existiert:
        QString promat_name;
        promat_name  = TABNAME_PROMAT;
        promat_name += projektid;
        text_zeilenweise artikelids = dbeigen->get_data_tz(promat_name, PARAM_PROMAT_ARTIKEL_ID);
        int index = -1;
        for(uint i=1; i<=artikelids.zeilenanzahl() ;i++)
        {
            if(artikelids.zeile(i) == artikelid)
            {
                index = i;
                break;
            }
        }

        //Wert anpassen:
        if(index > 0)//Artikel bereits vorhanden in promat_*
        {
            text_zeilenweise promat_value_ids = dbeigen->get_data_tz(promat_name, PARAM_PROMAT_ID);
            QString promat_value_id = promat_value_ids.zeile(index);

            int menge_vorher = dbeigen->get_data_qstring(promat_name, PARAM_PROMAT_ME_VERARBEITET, promat_value_id).toInt();
            int menge_jetzt = menge.toInt();
            int menge_nachher = menge_vorher + menge_jetzt;
            dbeigen->data_edit(promat_name, PARAM_PROMAT_ME_VERARBEITET, int_to_qstring(menge_nachher), promat_value_id);
        }else
        {
            text_zeilenweise pa, val;

            pa.zeile_anhaengen(PARAM_PROMAT_ARTIKEL_ID);
            pa.zeile_anhaengen(PARAM_PROMAT_ME_VERARBEITET);

            val.zeile_anhaengen(artikelid);
            val.zeile_anhaengen(menge);
            dbeigen->data_new(promat_name, pa, val);
        }

        //reservierte menge min artikel-tabelle verringern:
        int restervierte_menge = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, artikelid).toInt();
        if(restervierte_menge >= menge.toInt())
        {
            restervierte_menge = restervierte_menge - menge.toInt();
            dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                               int_to_qstring(restervierte_menge), artikelid);
        }else
        {
            restervierte_menge = 0;
            dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                               int_to_qstring(restervierte_menge), artikelid);
        }
        QMessageBox mb;
        mb.setText(tr("Buchung erfolgreich durchgeführt."));
        mb.exec();
    }else
    {
        QString msg;
        msg += "Der Artikel ist derzeit noch ";
        msg += menge_vorher;
        msg += " x im Lager.\n";
        msg += "Sie können nicht ";
        msg += menge;
        msg += " x entnehmen!";

        QMessageBox mb;
        mb.setText(tr(msg.toStdString().c_str()));
        mb.exec();
    }
}

void Form_lager::slot_inagain(text_zeilenweise data)
{
    text_zeilenweise param, values;

    QString artikelid   = data.zeile(1);
    QString menge       = data.zeile(2);
    QString projektid   = data.zeile(3);
    QString kommentar   = data.zeile(4);

    param.zeile_anhaengen(PARAM_LAGER_VORGANG);
    param.zeile_anhaengen(PARAM_LAGER_ARTIKELID);
    param.zeile_anhaengen(PARAM_LAGER_MENGE);
    param.zeile_anhaengen(PARAM_LAGER_ERSTELLER);
    param.zeile_anhaengen(PARAM_LAGER_DATERST);
    param.zeile_anhaengen(PARAM_LAGER_KOMMISSION);
    param.zeile_anhaengen(PARAM_LAGER_KOMMENT);

    values.zeile_anhaengen(VORGANG_WARENAUSGANG);
    values.zeile_anhaengen(artikelid);
    values.zeile_anhaengen(menge);
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d());
    values.zeile_anhaengen(projektid);
    values.zeile_anhaengen(kommentar);

    //Aktuelle Lagermenge des Artikeln abfragen:
    QString menge_vorher = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, artikelid);
    int menge_akt = menge_vorher.toInt() + menge.toInt();

    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, int_to_qstring(menge_akt), artikelid);

    dbeigen->data_new(TABNAME_LAGER, param, values);
    update_table();

    //heraus bekommen, ob zu diesem Artikel bereits ein Eintrag in der promat_* existiert:
    QString promat_name;
    promat_name  = TABNAME_PROMAT;
    promat_name += projektid;
    text_zeilenweise artikelids = dbeigen->get_data_tz(promat_name, PARAM_PROMAT_ARTIKEL_ID);
    int index = -1;
    for(uint i=1; i<=artikelids.zeilenanzahl() ;i++)
    {
        if(artikelids.zeile(i) == artikelid)
        {
            index = i;
            break;
        }
    }

    //Wert anpassen:
    if(index > 0)//Artikel bereits vorhanden in promat_*
    {
        text_zeilenweise promat_value_ids = dbeigen->get_data_tz(promat_name, PARAM_PROMAT_ID);
        QString promat_value_id = promat_value_ids.zeile(index);

        int menge_vorher = dbeigen->get_data_qstring(promat_name, PARAM_PROMAT_ME_VERARBEITET, promat_value_id).toInt();
        int menge_jetzt = menge.toInt();
        int menge_nachher = menge_vorher - menge_jetzt;
        dbeigen->data_edit(promat_name, PARAM_PROMAT_ME_VERARBEITET, int_to_qstring(menge_nachher), promat_value_id);
        if(menge_nachher < 0)
        {
            QMessageBox mb;
            mb.setText(tr("Es wurde mehr ins Lager zurückgegeben als zuvor für das Projekt ausgegeben wurde."));
            mb.exec();
        }
    }else
    {
        text_zeilenweise pa, val;

        pa.zeile_anhaengen(PARAM_PROMAT_ARTIKEL_ID);
        pa.zeile_anhaengen(PARAM_PROMAT_ME_VERARBEITET);

        val.zeile_anhaengen(artikelid);
        val.zeile_anhaengen("-" + menge);//es wurde mehr Wieder-Eingelagert als rausgegeben wurde
        dbeigen->data_new(promat_name, pa, val);
        QMessageBox mb;
        mb.setText(tr("Es wurde mehr ins Lager zurückgegeben als zuvor für das Projekt ausgegeben wurde."));
        mb.exec();
    }
    QMessageBox mb;
    mb.setText(tr("Buchung erfolgreich durchgeführt."));
    mb.exec();
}

void Form_lager::slot_bestkor_aid(QString artikel_id)
{
    //Prüfen, ob es eine Bestellung mit dieser ID gibt:
    text_zeilenweise artikel_ids = dbeigen->get_data_tz(TABNAME_ARTIKEL, PARAM_ARTIKEL_ID);
    bool existiert = false;
    for(uint i=1; i<=artikel_ids.zeilenanzahl() ;i++)
    {
        if(artikel_ids.zeile(i) == artikel_id)
        {
            existiert = true;
            break;
        }
    }
    if(existiert == true)
    {
        idbuffer = artikel_id;
    }else
    {
        idbuffer.clear();
        QMessageBox mb;
        mb.setText(tr("Ein Artikel mit dieser ID existiert nicht!"));
        mb.exec();
    }

}

void Form_lager::slot_bestkor_menge(QString menge)
{
    if(!idbuffer.isEmpty())
    {
        //Dialog für Bestandskorrektur aufrufen
    }
}

//------------------------------------













