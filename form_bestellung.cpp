#include "form_bestellung.h"
#include "ui_form_bestellung.h"

Form_bestellung::Form_bestellung(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_bestellung)
{
    ui->setupUi(this);
    dbeigen = NULL;
    this->model = new QSqlQueryModel();
    this->model_lieferverzug = new QSqlQueryModel();
    ui->dateEdit_bis->setDate(QDate::currentDate());
}

Form_bestellung::~Form_bestellung()
{
    delete ui;
    delete model;
    delete model_lieferverzug;
}

void Form_bestellung::resizeEvent(QResizeEvent *event)
{
    int b, h;
    b = width();
    h = height();
    //Rand_li = 1 + Rand_re = 1 + Rand_mi = 2 == 4
    int b_re = b/5*2;
    if(b_re > 350)
    {
        b_re = 350;
    }
    int b_li = b - b_re - 4;
    int h_btn = ui->pushButton_bestellen->height();

    //---------------------------------------------------------------------------
    //links Zeiteingrenzung:
    ui->dateEdit_ab->move(1, 1);
    ui->dateEdit_bis->move(1+ui->dateEdit_ab->width(), 1);

    //links Suchleiste:
    ui->lineEdit_suche->setFixedWidth(b/6);
    ui->lineEdit_suche->setFixedHeight(h_btn);
    ui->lineEdit_suche->move(b_li - 1 - ui->lineEdit_suche->width()\
                             ,1);
    ui->label_suche->setFixedHeight(ui->lineEdit_suche->height());
    ui->label_suche->move(b_li - 1 -ui->lineEdit_suche->width() - 1\
                          - ui->label_suche->width()\
                          ,1);

    //links Tabelle oben:
    int tabh = h - h_btn -1;
    int tabhob = tabh/8*5;
    int tabhun = tabh - tabhob - 5 - h_btn - 1;
    ui->tableView->move(1,\
                        1 + h_btn + 1);
    ui->tableView->setFixedWidth(b_li);
    ui->tableView->setFixedHeight(tabhob);

    //links Tabelle unten:
    ui->tableView_lieferverzug->move(1,\
                                1 + h_btn + 1 + tabhob + 5 + ui->label_lieferverzug->height() + 1);
    ui->tableView_lieferverzug->setFixedWidth(b_li);
    ui->tableView_lieferverzug->setFixedHeight(tabhun);

    ui->label_lieferverzug->setFixedHeight(h_btn);
    ui->label_lieferverzug->move(1,\
                                 ui->tableView_lieferverzug->pos().y() - h_btn - 1);
    ui->pushButton_change_LT->move(b_li - ui->pushButton_change_LT->width(),\
                                   ui->label_lieferverzug->pos().y());

    //rechts Label über Suchleiste:
    ui->label_bestellvorschlag->setFixedWidth(b_re);
    ui->label_bestellvorschlag->setFixedHeight(h_btn);
    ui->label_bestellvorschlag->move(1 + b_li + 2,\
                                     1);
    //---------------------------------------------------------------------------
    //rechts Suchleiste:
    ui->lineEdit_suche_vorschlag->setFixedWidth(b_re);
    ui->lineEdit_suche_vorschlag->setFixedHeight(h_btn);
    ui->lineEdit_suche_vorschlag->move(1 + b_li + 2,\
                                       1 + h_btn + 1);

    //rechts combobox Lieferanten:
    ui->comboBox_vorschlag_lieferant->setFixedWidth(b_re);
    ui->comboBox_vorschlag_lieferant->setFixedHeight(h_btn);
    ui->comboBox_vorschlag_lieferant->move(1 + b_li + 2,\
                                           1 + (h_btn + 1)*2);

    //rechts Listwidget Bestellvorschläge:
    ui->listWidget_vorschlag->setFixedWidth(b_re);
    ui->listWidget_vorschlag->setFixedHeight(h - (1 + h_btn)*4 - 2);
    ui->listWidget_vorschlag->move(1 + b_li + 2,\
                                   1 + (h_btn + 1)*3);

    //rechts buttons:
    ui->pushButton_bestellen->setFixedWidth(b_re/2-2);
    ui->pushButton_bestellung_stornieren->setFixedWidth(ui->pushButton_bestellen->width());

    ui->pushButton_bestellen->move(1 + b_li + 2,\
                                   1 + (h_btn + 1)*3 + 1 + ui->listWidget_vorschlag->height());
    ui->pushButton_bestellung_stornieren->move(1 + b_li + 2 + ui->pushButton_bestellen->width() + 1,\
                                               1 + (h_btn + 1)*3 + 1 + ui->listWidget_vorschlag->height());

    QWidget::resizeEvent(event);
}

void Form_bestellung::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_bestellung::set_user(QString u)
{
    user = u;
}

void Form_bestellung::show()
{
    update_bestellvor();
    update_bestellvor_tz();
    update_table();
    setVisible(true);
}

void Form_bestellung::update_table()
{
    if(dbeigen != NULL)
    {
        QString ab_datum, bis_datum;
        datum da;
        QDate qda;
        qda = ui->dateEdit_ab->date();
        ab_datum = da.get_datum_y_m_d(&qda);
        qda = ui->dateEdit_bis->date();
        bis_datum = da.get_datum_y_m_d(&qda);
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
                cmd += "SELECT ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ID;
                cmd += " AS ";
                cmd += "ID";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
                cmd += ", ";
                //------------------------
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += ", ";
                //------------------------
                cmd += TABNAME_LIEFERANT;
                cmd += ".";
                cmd += PARAM_LIEFERANT_NAME;
                cmd += " AS ";
                cmd += "Lieferant";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ME_BEST;
                cmd += " AS ";
                cmd += "bestellte_Menge";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ME_GELIEFERT;
                cmd += " AS ";
                cmd += "geliferte_Menge";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_PERSONAL;
                cmd += ".";
                cmd += PARAM_PERSONAL_NACHNAME;
                cmd += " AS ";
                cmd += "Ersteller";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_DATERST;
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_DATLIEF;
                //cmd += ", ";
                //------------------------
                cmd += " FROM ";
                cmd += TABNAME_BESTELLUNG;
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += TABNAME_ARTIKEL;
                cmd += " ON (";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ARTIKELID;
                cmd += " = ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_ID;
                cmd += ")";
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += TABNAME_LIEFERANT;
                cmd += " ON (";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_LIEFERANT;
                cmd += " = ";
                cmd += TABNAME_LIEFERANT;
                cmd += ".";
                cmd += PARAM_LIEFERANT_ID;
                cmd += ")";
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += TABNAME_PERSONAL;
                cmd += " ON (";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ERSTELLER;
                cmd += " = ";
                cmd += TABNAME_PERSONAL;
                cmd += ".";
                cmd += PARAM_PERSONAL_ID;
                cmd += ")";
                //------------------------
                //------------------------

                cmd += " WHERE ";
                cmd += "(";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_DATERST;
                cmd += " >= ";
                cmd += "\'";
                cmd += ab_datum;
                cmd += "\'";
                cmd += ")";
                cmd += " AND ";
                cmd += "(";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_DATERST;
                cmd += " <= ";
                cmd += "\'";
                cmd += bis_datum;
                cmd += "\'";
                cmd += ")";

                if(!ui->lineEdit_suche->text().isEmpty())
                {
                    cmd += " AND ";
                    cmd += "(";
                    cmd += PARAM_ARTIKEL_NR;
                    cmd += " LIKE \'%";
                    cmd += ui->lineEdit_suche->text();
                    cmd += "%\'";
                    cmd += " OR ";
                    cmd += PARAM_ARTIKEL_BEZ;
                    cmd += " LIKE \'%";
                    cmd += ui->lineEdit_suche->text();
                    cmd += "%\'";
                    cmd += ")";
                }
                //------------------------
                cmd += " GROUP BY ";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ID;
                //------------------------
                cmd += " ORDER BY ";            //Sortiert nach:
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ID;
                cmd += " DESC";
                //------------------------

                if(q.exec(cmd))
                {
                    model->setQuery(q);
                    ui->tableView->setModel(model);

                    QString msg;
                    msg += int_to_qstring(model->rowCount());
                    msg += " Bestellungen:";
                    ui->label_suche->setText(msg);

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
                mb.setText(tr("Fehler bei Datenbankverbindung!"));
                mb.exec();
            }
        }
        //-------------------------------------------
        update_table_lieferverzug();
    }
}

void Form_bestellung::update_table_lieferverzug()
{
    if(dbeigen != NULL)
    {
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
                cmd += "SELECT ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ID;
                cmd += " AS ";
                cmd += "ID";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
                cmd += ", ";
                //------------------------
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += ", ";
                //------------------------
                cmd += TABNAME_LIEFERANT;
                cmd += ".";
                cmd += PARAM_LIEFERANT_NAME;
                cmd += " AS ";
                cmd += "Lieferant";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ME_BEST;
                cmd += " AS ";
                cmd += "bestellte_Menge";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ME_GELIEFERT;
                cmd += " AS ";
                cmd += "geliferte_Menge";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_PERSONAL;
                cmd += ".";
                cmd += PARAM_PERSONAL_NACHNAME;
                cmd += " AS ";
                cmd += "Ersteller";
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_DATERST;
                cmd += ", ";
                //------------------------
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_DATLIEF;
                //cmd += ", ";
                //------------------------
                cmd += " FROM ";
                cmd += TABNAME_BESTELLUNG;
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += TABNAME_ARTIKEL;
                cmd += " ON (";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ARTIKELID;
                cmd += " = ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_ID;
                cmd += ")";
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += TABNAME_LIEFERANT;
                cmd += " ON (";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_LIEFERANT;
                cmd += " = ";
                cmd += TABNAME_LIEFERANT;
                cmd += ".";
                cmd += PARAM_LIEFERANT_ID;
                cmd += ")";
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += TABNAME_PERSONAL;
                cmd += " ON (";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ERSTELLER;
                cmd += " = ";
                cmd += TABNAME_PERSONAL;
                cmd += ".";
                cmd += PARAM_PERSONAL_ID;
                cmd += ")";
                //------------------------
                //------------------------
                cmd += " WHERE ";
                cmd += "(";
                cmd += PARAM_BESTELLUNG_DATLIEF;
                cmd += " < ";
                cmd += "NOW() ";
                cmd += ")";
                cmd += " AND ";
                cmd += "(";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ME_BEST;
                cmd += " > ";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ME_GELIEFERT;
                cmd += ")";
                //------------------------
                cmd += " GROUP BY ";
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ID;
                //------------------------
                cmd += " ORDER BY ";            //Sortiert nach:
                cmd += TABNAME_BESTELLUNG;
                cmd += ".";
                cmd += PARAM_BESTELLUNG_ID;
                cmd += " DESC";
                //------------------------

                if(q.exec(cmd))
                {
                    model_lieferverzug->setQuery(q);
                    ui->tableView_lieferverzug->setModel(model_lieferverzug);
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
                mb.setText(tr("Fehler bei Datenbankverbindung!"));
                mb.exec();
            }
        }
        //-------------------------------------------
    }
}

void Form_bestellung::update_bestellvor()
{
    //in dieser Funktion geht es darum zu viel Reserviertes zu stornieren
    //damit Bestellvorschläge möglichst vom wirklichen Bedarf erfasst sind

    if(dbeigen != NULL)
    {
        text_zeilenweise tabnames = dbeigen->get_tables_tz();
        text_zeilenweise tmp_tz;
        for(uint i=1; i<=tabnames.zeilenanzahl() ;i++)
        {
            QString name = tabnames.zeile(i);
            if(name.contains(TABNAME_PROMAT))
            {
                tmp_tz.zeile_anhaengen(name);
            }
        }
        tabnames = tmp_tz;
        for(uint i=1; i<=tabnames.zeilenanzahl() ;i++)
        {
            QString tabname = tabnames.zeile(i);
            text_zeilenweise ids, vorgemerkt, reserviert, artikel_id;
            ids         = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ID);
            vorgemerkt  = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ME_ZURBEST);
            reserviert  = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ME_RESERVIERT);
            artikel_id  = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ARTIKEL_ID);
            for(uint ii=1; ii<=ids.zeilenanzahl() ;ii++)
            {
                int vor = vorgemerkt.zeile(ii).toInt();
                int res = reserviert.zeile(ii).toInt();

                if(res > vor)
                {
                    int art_resme  = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                                               artikel_id.zeile(ii)).toInt();
                    int zustor = res - vor;

                    if(art_resme >= zustor)
                    {
                        //restervierung stornieren:
                        int neue_res_me_promat = vor;
                        int neue_res_me_artikel = art_resme - zustor;
                        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                           int_to_qstring(neue_res_me_artikel), artikel_id.zeile(ii));
                        dbeigen->data_edit(tabname, PARAM_PROMAT_ME_RESERVIERT, \
                                           int_to_qstring(neue_res_me_promat), ids.zeile(ii));
                    }else
                    {
                        //Teilmenge der restervierung stornieren:
                        int neue_res_me_promat =  vor + zustor - art_resme;
                        int neue_res_me_artikel = 0;
                        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                           int_to_qstring(neue_res_me_artikel), artikel_id.zeile(ii));
                        dbeigen->data_edit(tabname, PARAM_PROMAT_ME_RESERVIERT, \
                                           int_to_qstring(neue_res_me_promat), ids.zeile(ii));
                    }
                }
            }
        }
    }
}

void Form_bestellung::update_bestellvor_tz()
{
    besvor_artikel_id.clear();
    besvor_artikel_nr.clear();
    besvor_artikel_bez.clear();
    besvor_menge.clear();
    besvor_artikel_lieferant.clear();

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
            cmd += "SELECT ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_NR;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTVOR;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LIEFERANT;
            //cmd += ", ";
            //------------------------
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;
            //------------------------
            //------------------------
            cmd += " WHERE ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTVOR;
            cmd += " > 0";
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
            //------------------------

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    besvor_artikel_id.zeile_anhaengen(q.value(0).toString());
                    besvor_artikel_nr.zeile_anhaengen(q.value(1).toString());
                    besvor_artikel_bez.zeile_anhaengen(q.value(2).toString());
                    besvor_menge.zeile_anhaengen(q.value(3).toString());
                    besvor_artikel_lieferant.zeile_anhaengen(q.value(4).toString());
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
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
        update_bestellvor_listwidget();
    }
    //-------------------------------------------

}

void Form_bestellung::update_bestellvor_listwidget()
{
    //------------------------------------------------------------------------------
    besvor_filtern();
    //------------------------------------------------------------------------------
    //Lieferanten filtern:
    text_zeilenweise lieferanten;
    if(besvor_artikel_lieferant_gefiltert.zeilenanzahl() > 0)
    {
        for(uint i=1; i<=besvor_artikel_lieferant_gefiltert.zeilenanzahl();i++)
        {
            QString li_id = besvor_artikel_lieferant_gefiltert.zeile(i);
            bool vorhanden = false;
            for(uint ii=1; ii<=lieferanten.zeilenanzahl() ;ii++)
            {
                if(li_id == lieferanten.zeile(ii))
                {
                    vorhanden = true;
                    break;
                }
            }
            if(vorhanden == false)
            {
                lieferanten.zeile_anhaengen(li_id);
            }
        }
    }
    ui->comboBox_vorschlag_lieferant->clear();
    ui->comboBox_vorschlag_lieferant->addItem("ALLE");
    for(uint i=1; i<=lieferanten.zeilenanzahl() ;i++)
    {
        QString liname;
        liname += lieferanten.zeile(i);
        liname += " ||| ";
        liname += dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_NAME, lieferanten.zeile(i));
        ui->comboBox_vorschlag_lieferant->addItem(liname);
    }
    //------------------------------------------------------------------------------
    update_bestellvor_listwidget_items();
    //------------------------------------------------------------------------------
    QString labeltext;
    labeltext += int_to_qstring(ui->listWidget_vorschlag->count());
    labeltext += " Bestellvorschläge  ";
    ui->label_bestellvorschlag->setText(labeltext);
    //------------------------------------------------------------------------------
}

void Form_bestellung::update_bestellvor_listwidget_items()
{
    ui->listWidget_vorschlag->clear();
    for(uint i=1; i<=besvor_artikel_id_gefiltert.zeilenanzahl() ;i++)
    {
        QString zeile;
        zeile += besvor_menge_gefiltert.zeile(i);
        zeile += "x   ";
        zeile += besvor_artikel_nr_gefiltert.zeile(i);
        zeile += " | ";
        zeile += besvor_artikel_bez_gefiltert.zeile(i);
        ui->listWidget_vorschlag->addItem(zeile);
    }
}

void Form_bestellung::besvor_filtern()
{
    if(ui->lineEdit_suche_vorschlag->text().isEmpty())
    {
        besvor_artikel_id_gefiltert = besvor_artikel_id;
        besvor_artikel_nr_gefiltert = besvor_artikel_nr;
        besvor_artikel_bez_gefiltert = besvor_artikel_bez;
        besvor_menge_gefiltert = besvor_menge;
        besvor_artikel_lieferant_gefiltert = besvor_artikel_lieferant;
    }else
    {
        besvor_artikel_id_gefiltert.clear();
        besvor_artikel_nr_gefiltert.clear();
        besvor_artikel_bez_gefiltert.clear();
        besvor_menge_gefiltert.clear();
        besvor_artikel_lieferant_gefiltert.clear();
        for(uint i=1; i<=besvor_artikel_id.zeilenanzahl() ;i++)
        {
            QString bez, nr, suchbegriff;
            bez = besvor_artikel_bez.zeile(i).toUpper();
            nr  = besvor_artikel_nr.zeile(i).toUpper();
            suchbegriff = ui->lineEdit_suche_vorschlag->text().toUpper();
            if(nr.contains(suchbegriff)  ||  bez.contains(suchbegriff))
            {
                besvor_artikel_id_gefiltert.zeile_anhaengen(besvor_artikel_id.zeile(i));
                besvor_artikel_nr_gefiltert.zeile_anhaengen(besvor_artikel_nr.zeile(i));
                besvor_artikel_bez_gefiltert.zeile_anhaengen(besvor_artikel_bez.zeile(i));
                besvor_menge_gefiltert.zeile_anhaengen(besvor_menge.zeile(i));
                besvor_artikel_lieferant_gefiltert.zeile_anhaengen(besvor_artikel_lieferant.zeile(i));
            }
        }
    }
}

void Form_bestellung::artikelupdate_bestellvorschlag(QString artikel_id)
{
    //Freie Menge berechnen == welche Stückzahl vom Atrikel ist noch nicht verplant:
    int lagerbestand    = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, \
                                                    artikel_id).toInt();
    int in_bestellung   = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, \
                                                    artikel_id).toInt();
    int reserviert      = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                                    artikel_id).toInt();
    int frei = lagerbestand + in_bestellung - reserviert;
    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_FREI, \
                       int_to_qstring(frei), artikel_id);

    //Bestellvorschlag ermitteln und speichern:
    int mindestbestand  = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERST_MIN, \
                                                    artikel_id).toInt();
    int ve              = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_VE, \
                                                    artikel_id).toInt();

    if( (lagerbestand + in_bestellung - mindestbestand) < reserviert  )
    {
        int x = lagerbestand + in_bestellung - mindestbestand - reserviert;
        int anz_ves     = -x/ve;
        int rest        = -x%ve;
        if(rest != 0)
        {
            anz_ves++;
        }
        int bestellvorschlag = anz_ves * ve;
        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTVOR, \
                           int_to_qstring(bestellvorschlag), artikel_id);
    }else
    {
        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTVOR, \
                           "0", artikel_id);
    }
}

//------------------------------------Buttons:
void Form_bestellung::on_pushButton_bestellen_clicked()
{
    if(ui->listWidget_vorschlag->currentRow() != -1)
    {
        QString artikel_id, artikel_nr, artikel_bez, lieferant, ve, menge, liefertermin, kommentar;
        artikel_id      = besvor_artikel_id_gefiltert.zeile(ui->listWidget_vorschlag->currentRow()+1);
        artikel_nr      = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_NR, artikel_id);
        artikel_bez     = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZ, artikel_id);
        lieferant       = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LIEFERANT, artikel_id);
        lieferant       = dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_NAME, lieferant);
        ve              = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_VE, artikel_id);
        menge           = besvor_menge_gefiltert.zeile(ui->listWidget_vorschlag->currentRow()+1);
        liefertermin    = "0";
        kommentar       = "";

        text_zeilenweise data;
        data.zeile_anhaengen(artikel_id);
        data.zeile_anhaengen(artikel_nr);
        data.zeile_anhaengen(artikel_bez);
        data.zeile_anhaengen(lieferant);
        data.zeile_anhaengen(ve);
        data.zeile_anhaengen(menge);
        data.zeile_anhaengen(liefertermin);
        data.zeile_anhaengen(kommentar);

        Dialog_bestellung *d = new Dialog_bestellung(this);
        d->setWindowTitle("Bestellen");
        d->set_data(data);
        connect(d, SIGNAL(signal_send_data(text_zeilenweise)),      \
                this, SLOT(slot_neue_bestellung(text_zeilenweise))  );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst einen Bestellvorschlag wählen!"));
        mb.exec();
    }
}

void Form_bestellung::on_pushButton_bestellung_stornieren_clicked()
{
    Dialog_text_input *d = new Dialog_text_input(this);
    d->setup("Stornieren", "Bitte geben Sie die ID-Nummer der zu stornierenden Bestellung ein:");
    connect(d, SIGNAL(signal_userinput(QString)),           \
            this, SLOT(slot_bestellung_stornieren(QString)) );
    d->exec();
    delete d;
}

void Form_bestellung::on_pushButton_change_LT_clicked()
{
    Dialog_text_input *d = new Dialog_text_input(this);
    d->setup("Terminänderung", "Bitte geben Sie die ID-Nummer der betreffenden Bestellung ein:");
    connect(d, SIGNAL(signal_userinput(QString)),           \
            this, SLOT(slot_change_LT(QString)) );
    d->exec();
    delete d;
}

//------------------------------------private slots:
void Form_bestellung::on_lineEdit_suche_textChanged(const QString &arg1)
{
    update_table();
}

void Form_bestellung::on_lineEdit_suche_vorschlag_textChanged(const QString &arg1)
{
    update_bestellvor_listwidget();
}

void Form_bestellung::on_comboBox_vorschlag_lieferant_currentIndexChanged(const QString &arg1)
{
    if(ui->comboBox_vorschlag_lieferant->count() > 0)
    {
        besvor_filtern();
        if(arg1 != "ALLE")
        {
            //Gefilterte Lieste zwischenspeichern
            text_zeilenweise aid, anr, abez, men, lid;
            aid = besvor_artikel_id_gefiltert;
            anr = besvor_artikel_nr_gefiltert;
            abez = besvor_artikel_bez_gefiltert;
            men = besvor_menge_gefiltert;
            lid = besvor_artikel_lieferant_gefiltert;

            //Gefilterte Lieste leeren
            besvor_artikel_id_gefiltert.clear();
            besvor_artikel_nr_gefiltert.clear();
            besvor_artikel_bez_gefiltert.clear();
            besvor_menge_gefiltert.clear();
            besvor_artikel_lieferant_gefiltert.clear();

            QString comliid = text_links(arg1, " ||| ");

            for(uint i=1; i<=aid.zeilenanzahl() ;i++)
            {
                if(lid.zeile(i) == comliid)
                {
                    besvor_artikel_id_gefiltert.zeile_anhaengen(aid.zeile(i));
                    besvor_artikel_nr_gefiltert.zeile_anhaengen(anr.zeile(i));
                    besvor_artikel_bez_gefiltert.zeile_anhaengen(abez.zeile(i));
                    besvor_menge_gefiltert.zeile_anhaengen(men.zeile(i));
                    besvor_artikel_lieferant_gefiltert.zeile_anhaengen(lid.zeile(i));
                }
            }
        }
        update_bestellvor_listwidget_items();
    }

}

void Form_bestellung::on_dateEdit_ab_userDateChanged(const QDate &date)
{
    update_table();
}

void Form_bestellung::on_dateEdit_bis_userDateChanged(const QDate &date)
{
    update_table();
}

//------------------------------------public slots:
void Form_bestellung::slot_neue_bestellung(text_zeilenweise data)
{
    QString artikel_id, menge, liefertermin;
    artikel_id      = data.zeile(1);
    menge           = data.zeile(2);
    liefertermin    = data.zeile(3);

    datum heute;

    text_zeilenweise pa,val;
    pa.zeile_anhaengen(PARAM_BESTELLUNG_ARTIKELID);
    pa.zeile_anhaengen(PARAM_BESTELLUNG_ME_BEST);
    pa.zeile_anhaengen(PARAM_BESTELLUNG_DATERST);
    pa.zeile_anhaengen(PARAM_BESTELLUNG_ERSTELLER);
    pa.zeile_anhaengen(PARAM_BESTELLUNG_ME_GELIEFERT);

    val.zeile_anhaengen(artikel_id);
    val.zeile_anhaengen(menge);
    val.zeile_anhaengen(heute.get_today_y_m_d());
    val.zeile_anhaengen(user);
    val.zeile_anhaengen("0");

    dbeigen->data_new(TABNAME_BESTELLUNG, pa, val);

    QString id = dbeigen->get_highest_id(TABNAME_BESTELLUNG);
    if(liefertermin == "0")
    {
        dbeigen->data_edit(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_DATLIEF, "", id);
    }else
    {
        dbeigen->data_edit(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_DATLIEF, liefertermin, id);
    }
    dbeigen->data_edit(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_KOMMENT, liefertermin, id);

    //-----------------------------------------------------
    //Bestellte Menge im Artikel erhöhen:
    int artikel_menge_bestellt = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, artikel_id).toInt();
    artikel_menge_bestellt += menge.toInt();
    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, \
                       double_to_qstring(artikel_menge_bestellt), artikel_id);
    //-----------------------------------------------------
    artikelupdate_bestellvorschlag(artikel_id);
    update_bestellvor();
    update_bestellvor_tz();
    update_table();
}

void Form_bestellung::slot_bestellung_stornieren(QString bestell_id)
{
    //Prüfen, ob es eine Bestellung mit dieser ID gibt:
    text_zeilenweise bestell_ids = dbeigen->get_data_tz(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ID);
    bool existiert = false;
    for(uint i=1; i<=bestell_ids.zeilenanzahl() ;i++)
    {
        if(bestell_ids.zeile(i) == bestell_id)
        {
            existiert = true;
            break;
        }
    }
    if(existiert == true)
    {
        int me_geliefert = dbeigen->get_data_qstring(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ME_GELIEFERT, bestell_id).toInt();
        if(me_geliefert == 0)
        {
            int me_bestellt = dbeigen->get_data_qstring(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ME_BEST, bestell_id).toInt();
            QString artikel_id = dbeigen->get_data_qstring(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ARTIKELID, bestell_id);
            int artikel_in_bestellung = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, artikel_id).toInt();
            if(me_bestellt <= artikel_in_bestellung)
            {
                //Bestellte Menge im Artikel verringern:
                artikel_in_bestellung = artikel_in_bestellung - me_bestellt;
                dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, \
                                   int_to_qstring(artikel_in_bestellung), artikel_id);
                //Bestellung löschen:
                dbeigen->data_del(TABNAME_BESTELLUNG, bestell_id);
                artikelupdate_bestellvorschlag(artikel_id);
                update_bestellvor();
                update_bestellvor_tz();
                update_table();
            }else
            {
                //Diese Stelle sollte nie erreicht werden!
                QMessageBox mb;
                mb.setText(tr("Fehler im Datensatz!\nBestellung kann nicht storniert werden!\nBitte benachrichtigen sie ihren Administrator!!"));
                mb.exec();
            }
        }else
        {
            int me_bestellt = dbeigen->get_data_qstring(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ME_BEST, bestell_id).toInt();
            if(me_bestellt != me_geliefert)
            {
                QMessageBox mb;
                mb.setText(tr("Die Bestellung kann nicht storniert werden, weil das Bestellte bereits teilweise geliefert wurde"));
                mb.exec();
            }else
            {
                QMessageBox mb;
                mb.setText(tr("Die Bestellung kann nicht storniert werden, weil das Bestellte bereits vollständig geliefert wurde"));
                mb.exec();
            }
        }
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Eine Bestellung mit dieser ID existiert nicht!"));
        mb.exec();
    }
}

void Form_bestellung::slot_change_LT(QString bestell_id)
{
    //Prüfen, ob es eine Bestellung mit dieser ID gibt:
    text_zeilenweise bestell_ids = dbeigen->get_data_tz(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_ID);
    bool existiert = false;
    for(uint i=1; i<=bestell_ids.zeilenanzahl() ;i++)
    {
        if(bestell_ids.zeile(i) == bestell_id)
        {
            existiert = true;
            break;
        }
    }
    if(existiert == true)
    {
        idbuffer = bestell_id;
        QString termin = dbeigen->get_data_qstring(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_DATLIEF, bestell_id);
        int jahr, monat, tag;
        jahr    = text_links(termin, "-").toInt();
        monat   = text_mitte(termin, "-", "-").toInt();
        QString tmp = text_rechts(termin, "-");
        tmp = text_rechts(tmp, "-");
        tag     = tmp.toInt();
        QDate date;
        date.setDate(jahr, monat, tag);

        Dialog_datum_input *d = new Dialog_datum_input(this);
        d->setWindowTitle("Neuer Liefertermin");
        d->set_date(date);
        connect(d, SIGNAL(signal_send_date(QDate*)),     \
                this, SLOT(slot_change_LT(QDate*))       );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Eine Bestellung mit dieser ID existiert nicht!"));
        mb.exec();
    }
}

void Form_bestellung::slot_change_LT(QDate *da)
{
    datum dat;
    QString neues_datum = dat.get_datum_y_m_d(da);
    dbeigen->data_edit(TABNAME_BESTELLUNG, PARAM_BESTELLUNG_DATLIEF, neues_datum, idbuffer);
    update_table();
}

//------------------------------------
















