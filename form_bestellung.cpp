#include "form_bestellung.h"
#include "ui_form_bestellung.h"

Form_bestellung::Form_bestellung(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_bestellung)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_bestellung::~Form_bestellung()
{
    delete ui;
    delete model;
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
    int h_btn = ui->lineEdit_suche->height();

    //links Suchleiste:
    ui->lineEdit_suche->setFixedWidth(b/3);
    ui->lineEdit_suche->setFixedHeight(h_btn);
    ui->lineEdit_suche->move(b_li - 1 - ui->lineEdit_suche->width()\
                             ,1);
    ui->label_suche->setFixedHeight(ui->lineEdit_suche->height());
    ui->label_suche->move(b_li - 1 -ui->lineEdit_suche->width() - 1\
                          - ui->label_suche->width()\
                          ,1);

    //links Tabelle:
    ui->tableView->move(1,\
                        1 + h_btn + 1);
    ui->tableView->setFixedWidth(b_li);
    ui->tableView->setFixedHeight(h - ui->tableView->pos().ry() -1);

    //rechts Label über Suchleiste:
    ui->label_bestellvorschlag->setFixedWidth(b_re);
    ui->label_bestellvorschlag->setFixedHeight(h_btn);
    ui->label_bestellvorschlag->move(1 + b_li + 2,\
                                     1);

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
    ui->listWidget_vorschlag->setFixedHeight(h - (1 - h_btn)*3 - 2);
    ui->listWidget_vorschlag->move(1 + b_li + 2,\
                                   1 + (h_btn + 1)*3);

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
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_PREIS;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LAGERSTAND;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LAGERST_MIN;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_VE;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTVOR;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTELLT;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_KOMENT;
            //cmd += ", ";
            //------------------------

            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;
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
            //------------------------
            cmd += " WHERE ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTVOR;
            cmd += " > 0";

            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " AND";
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
                model->setQuery(q);
                ui->tableView->setModel(model);

                QString msg;
                msg += int_to_qstring(model->rowCount());
                msg += " Artikel:";
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
}

void Form_bestellung::update_bestellvor()
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

void Form_bestellung::update_bestellvor_tz()
{
    besvor_artikel_id.clear();
    besvor_artikel_nr.clear();
    besvor_artikel_bez.clear();
    besvor_menge.clear();
    besvor_artikel_lieferant.clear();

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

//------------------------------------Buttons:

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

//------------------------------------public slots:

//------------------------------------






