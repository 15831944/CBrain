#include "form_matlist.h"
#include "ui_form_matlist.h"

Form_matlist::Form_matlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_matlist)
{
    ui->setupUi(this);
    dbeigen = NULL;
    ui->lineEdit_projekt->setText("Projekt?");
    this->model = new QSqlQueryModel();
}

Form_matlist::~Form_matlist()
{
    delete ui;
    delete model;
}

void Form_matlist::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_matlist::set_user(QString u)
{
    user = u;
}

void Form_matlist::set_proid(QString projektid)
{
    text_zeilenweise pro;
    pro.set_text(projektid);
    slot_set_project(pro);
}

void Form_matlist::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->height();
    int breite = this->width();

    //Rand_li = 1 + Rand_re = 1 + Rand_mi = 2 == 4
    int b_re = breite/5*2;
    if(b_re > 250)
    {
        b_re = 250;
    }
    int b_li = breite - b_re - 4;


    int h_btn = ui->pushButton_projektauswahl->height();

    //links Kopfzeile:
    ui->lineEdit_projekt_id->setFixedHeight(h_btn);
    ui->lineEdit_projekt_id->move(1,1);

    ui->lineEdit_projekt->setFixedHeight(h_btn);
    ui->lineEdit_projekt->move(1 + ui->lineEdit_projekt_id->width() + 1\
                               ,1);

    ui->pushButton_projektauswahl->move(1 + ui->lineEdit_projekt_id->width() + \
                                        1 + ui->lineEdit_projekt->width() + 1,\
                                        1);

    ui->lineEdit_filter->setFixedHeight(h_btn);
    ui->lineEdit_filter->setFixedWidth(b_li/3);
    ui->lineEdit_filter->move(b_li - ui->lineEdit_filter->width(),\
                              1);
    ui->label_filter->setFixedHeight(h_btn);
    ui->label_filter->move(b_li - ui->lineEdit_filter->width() - 1 - ui->label_filter->width(),\
                           1);
    int restbr_li = b_li - 4 - ui->lineEdit_projekt->width() - ui->lineEdit_projekt_id->width() -\
                    ui->pushButton_projektauswahl->width() - ui->lineEdit_filter->width();
    if(restbr_li < ui->label_filter->width())
    {
        ui->label_filter->hide();
    }else
    {
        ui->label_filter->show();
    }

    //links 2. Zeile Button(s):
    ui->pushButton_reservieren->move(1,\
                                     1 + h_btn + 1);

    //links 2. Zeile Filter:
    ui->checkBox_erfasst->setFixedHeight(h_btn);
    ui->checkBox_unklar->setFixedHeight(h_btn);
    ui->checkBox_bestellen->setFixedHeight(h_btn);    

    ui->checkBox_erfasst->move(b_li - ui->checkBox_bestellen->width() \
                               - 1 - ui->checkBox_unklar->width()       \
                               -1 - ui->checkBox_erfasst->width(),\
                               1 + h_btn + 1);
    ui->checkBox_unklar->move(b_li - ui->checkBox_bestellen->width() \
                              - 1 - ui->checkBox_unklar->width(),\
                              1 + h_btn + 1);
    ui->checkBox_bestellen->move(b_li - ui->checkBox_bestellen->width(),\
                                 1 + h_btn + 1);

    //links 3. Zeile:
    ui->checkBox_einzelmengen->setFixedHeight(h_btn);
    ui->checkBox_reserviert->setFixedHeight(h_btn);
    ui->checkBox_verarbeitet->setFixedHeight(h_btn);
    ui->checkBox_pos_status->setFixedHeight(h_btn);

    ui->checkBox_einzelmengen->move(1,\
                                    1 + (h_btn + 1)*2                             );
    ui->checkBox_reserviert->move(1\
                                  + ui->checkBox_einzelmengen->width() +1 \
                                  ,\
                                  1 + (h_btn + 1)*2                             );
    ui->checkBox_verarbeitet->move(1\
                                  + ui->checkBox_einzelmengen->width() +1 \
                                   + ui->checkBox_reserviert->width() +1 \
                                  ,\
                                  1 + (h_btn + 1)*2                             );
    ui->checkBox_pos_status->move(1\
                                  + ui->checkBox_einzelmengen->width() +1 \
                                  + ui->checkBox_reserviert->width() +1 \
                                  + ui->checkBox_verarbeitet->width() +1 \
                                  ,\
                                  1 + (h_btn + 1)*2                             );

    //links Tabelle:
    ui->tableView->setFixedSize(b_li,\
                                hoehe - 1 - (h_btn+1)*4 - 1 );
    ui->tableView->move(1,\
                        1 + (h_btn + 1)*3);

    //links Statusleiste + Buttons:
    ui->lineEdit_statusbar->setFixedHeight(h_btn);
    ui->lineEdit_statusbar->setFixedWidth(b_li - 2 - ui->pushButton_packliste->width());
    ui->lineEdit_statusbar->move(1,\
                                 hoehe - h_btn);
    ui->pushButton_packliste->setFixedHeight(h_btn);
    ui->pushButton_packliste->move(b_li - ui->pushButton_packliste->width(),\
                                     hoehe - h_btn);

    //rechts Kopfzeile:
    ui->pushButton_check_all_pos->setFixedWidth(b_re/3-3);
    ui->pushButton_check_all_pos->move(1 + b_li + 2,\
                                       1);

    ui->pushButton_check_activ_pos->setFixedWidth(ui->pushButton_check_all_pos->width());
    ui->pushButton_check_activ_pos->move(1 + b_li + 2 + ui->pushButton_check_all_pos->width() + 1,\
                                         1);

    ui->pushButton_update_table->setFixedWidth(ui->pushButton_check_all_pos->width());
    ui->pushButton_update_table->move(1 + b_li + 2 + (ui->pushButton_check_all_pos->width() + 1)*2,\
                                      1);


    //rechts 2. Zeile:
    ui->checkBox_show_pos->setFixedHeight(h_btn);
    ui->checkBox_show_pos->move(1 + b_li + 2,\
                                1 + (h_btn + 1));

    //rechts Listwidget
    ui->listWidget_matpos->setFixedSize(b_re,\
                                        hoehe - 1*5 - h_btn*4);
    ui->listWidget_matpos->move(1 + b_li + 2,\
                                1 + (h_btn + 1)*2);

    //rechts Buttons unter Listwidget:
    ui->pushButton_pos_new->setFixedWidth(b_re/3 - 2*2);
    ui->pushButton_pos_delete->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_edit->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_copy->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_import->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_edit_rumpf->setFixedWidth(ui->pushButton_pos_new->width());

    ui->pushButton_pos_new->move(1 + b_li + 2,\
                                 1 + (h_btn + 1)*2 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_delete->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                    1 + (h_btn + 1)*2 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_edit->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                  1 + (h_btn + 1)*2 + ui->listWidget_matpos->height() + 1);

    ui->pushButton_pos_copy->move(1 + b_li + 2,\
                                  1 + (h_btn + 1)*2 + ui->listWidget_matpos->height() + 1 + h_btn + 1);
    ui->pushButton_pos_import->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                    1 + (h_btn + 1)*2 + ui->listWidget_matpos->height() + 1 + h_btn + 1);
    ui->pushButton_pos_edit_rumpf->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                        1 + (h_btn + 1)*2 + ui->listWidget_matpos->height() + 1 + h_btn + 1);

    QWidget::resizeEvent(event);
}

void Form_matlist::create_table_promatposlist()
{
    //Tabelle die die Materialpositionen erfasst

    if(!ui->lineEdit_projekt_id->text().isEmpty() && dbeigen != NULL)
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
            cmd += "CREATE TABLE ";
            cmd += "IF NOT EXISTS ";
            cmd += TABNAME_PROMATPOSLIST;
            cmd += ui->lineEdit_projekt_id->text();
            cmd += "(";
            cmd += "id int(11) unsigned auto_increment primary key not null";
            cmd += ", ";
            cmd += PARAM_PROMATPOSLIST_BEZ;
            cmd += " varchar(100)";
            cmd += ", ";
            cmd += PARAM_PROMATPOSLIST_MENGE;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOSLIST_ERSTELLER;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOSLIST_BEARBEITER;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOSLIST_BLOCK;
            cmd += " int(11) unsigned";
            cmd += ")";
            cmd += " ENGINE=InnoDB";

            if(!q.exec(cmd))
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
}

void Form_matlist::create_table_promatpos(QString bez, QString menge)
{
    //Tabelle die den Inhalt einer Materialposition erfasst

    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        //Eintrag in promatposlist erstellen:
        QString promatposlistname;
        promatposlistname  = TABNAME_PROMATPOSLIST;
        promatposlistname += ui->lineEdit_projekt_id->text();
        text_zeilenweise pa, val;

        pa.zeile_anhaengen(PARAM_PROMATPOSLIST_BEZ);
        pa.zeile_anhaengen(PARAM_PROMATPOSLIST_MENGE);
        pa.zeile_anhaengen(PARAM_PROMATPOSLIST_ERSTELLER);

        val.zeile_anhaengen(bez);
        val.zeile_anhaengen(menge);
        val.zeile_anhaengen(user);

        dbeigen->data_new(promatposlistname, pa, val);

        //neue promatpos-Tabelle erstellen:

        QString myid = dbeigen->get_highest_id(promatposlistname);

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
            cmd += "CREATE TABLE ";
            cmd += "IF NOT EXISTS ";
            cmd += TABNAME_PROMATPOSLIST;
            cmd += ui->lineEdit_projekt_id->text();
            cmd += "_";
            cmd += myid;
            cmd += "(";
            cmd += "id int(11) unsigned auto_increment primary key not null";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_ARTIKEL_ID;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_MENGE;
            cmd += " double";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_STATUS_ID;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_ERSTELLER;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_BEARBEITER;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_BLOCK;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_BEZIEHUNG;
            cmd += " tinytext";
            cmd += ")";
            cmd += " ENGINE=InnoDB";

            if(!q.exec(cmd))
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
}

void Form_matlist::update_listwidget_matpos()
{
    int current_row = ui->listWidget_matpos->currentRow();

    QString posliste;
    posliste  = TABNAME_PROMATPOSLIST;
    posliste += ui->lineEdit_projekt_id->text();

    //tabellen erfragen:
    text_zeilenweise table_ids;
    text_zeilenweise table_bez;

    table_ids = dbeigen->get_data_tz(posliste, PARAM_PROMATPOSLIST_ID);
    table_bez = dbeigen->get_data_tz(posliste, PARAM_PROMATPOSLIST_BEZ);

    ui->listWidget_matpos->clear();
    QListWidgetItem* item[table_ids.zeilenanzahl()];
    for(uint i=1; i<=table_ids.zeilenanzahl() ;i++)
    {
        QString itemname;
        itemname += table_ids.zeile(i);
        itemname += " ||| ";
        itemname += table_bez.zeile(i).replace(NEW_LINE_BR, "   ");
        item[i-1] = new QListWidgetItem(itemname  , ui->listWidget_matpos);
        item[i-1]->setFlags(item[i-1]->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item[i-1]->setCheckState(Qt::Checked); // AND initialize check state
        ui->listWidget_matpos->addItem(item[i-1]);
    }

    if(current_row != -1 && \
       ui->listWidget_matpos->count()-1 >= current_row)
    {
        ui->listWidget_matpos->setCurrentRow(current_row);
    }

}

void Form_matlist::update_table()
{
    //Namen aller Positionstabellen erfassen:
    QString posliste;
    posliste  = TABNAME_PROMATPOSLIST;
    posliste += ui->lineEdit_projekt_id->text();

    text_zeilenweise matpos_ids = dbeigen->get_data_tz(posliste, PARAM_PROMATPOSLIST_ID);
    text_zeilenweise names_postables;
    for(uint i=1; i<=matpos_ids.zeilenanzahl() ;i++)
    {
        if(ui->listWidget_matpos->item(i-1)->checkState() == Qt::Checked)
        {
            QString name;
            name += TABNAME_PROMATPOS;
            name += ui->lineEdit_projekt_id->text();
            name += "_";
            name += matpos_ids.zeile(i);
            names_postables.zeile_anhaengen(name);
        }
    }
    text_zeilenweise matpos_names_alle = dbeigen->get_data_tz(posliste, PARAM_PROMATPOSLIST_BEZ);
    text_zeilenweise matpos_names;
    for(uint i=1; i<=matpos_names_alle.zeilenanzahl() ;i++)
    {
        if(ui->listWidget_matpos->item(i-1)->checkState() == Qt::Checked)
        {
            QString name = matpos_names_alle.zeile(i);
            if(name.contains("\n"))
            {
                name = text_links(name, "\n");
            }
            name.replace(" ", "_");
            matpos_names.zeile_anhaengen(name);
        }
    }

    bool statusfilter_is_aktiv;
    if(!ui->checkBox_erfasst->isChecked()  ||   \
       !ui->checkBox_unklar->isChecked()   ||   \
       !ui->checkBox_bestellen->isChecked()     )
    {
        if(!ui->checkBox_erfasst->isChecked()  &&   \
           !ui->checkBox_unklar->isChecked()   &&   \
           !ui->checkBox_bestellen->isChecked()     )
        {
            statusfilter_is_aktiv = false;
        }else
        {
            statusfilter_is_aktiv = true;
        }
    }else
    {
        statusfilter_is_aktiv = false;
    }
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
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_NAME;
            cmd += " AS ";
            cmd += "Lieferant";
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
            cmd += PARAM_ARTIKEL_PREIS;
            cmd += " AS ";
            cmd += "Preis";
            cmd += ", ";
            //------------------------
            cmd += promat_tabname;
            cmd += ".";
            cmd += PARAM_PROMAT_ME_GESAMTBEDARF;
            cmd += ", ";
            //------------------------
            if(ui->checkBox_einzelmengen->isChecked())
            {
                //------------------------
                cmd += promat_tabname;
                cmd += ".";
                cmd += PARAM_PROMAT_ME_ERFASST;
                cmd += ", ";
                //------------------------
                cmd += promat_tabname;
                cmd += ".";
                cmd += PARAM_PROMAT_ME_UNKLAR;
                cmd += ", ";
                //------------------------
                cmd += promat_tabname;
                cmd += ".";
                cmd += PARAM_PROMAT_ME_ZURBEST;
                cmd += ", ";
                //------------------------
            }
            //------------------------
            if(ui->checkBox_reserviert->isChecked())
            {
                cmd += promat_tabname;
                cmd += ".";
                cmd += PARAM_PROMAT_ME_RESERVIERT;
                cmd += ", ";
            }
            //------------------------
            if(ui->checkBox_verarbeitet->isChecked())
            {
                cmd += promat_tabname;
                cmd += ".";
                cmd += PARAM_PROMAT_ME_VERARBEITET;
                cmd += ", ";
            }
            //------------------------
            //------------------------
            for(uint i=1; i<=names_postables.zeilenanzahl() ;i++)
            {
                if(ui->checkBox_show_pos->isChecked())
                {
                    //------------------------
                    cmd += names_postables.zeile(i);
                    cmd += ".";
                    cmd += PARAM_PROMATPOS_MENGE;
                    cmd += " AS ";
                    cmd += matpos_names.zeile(i);
                    cmd += ", ";
                    //------------------------
                    if(ui->checkBox_pos_status->isChecked() )
                    {
                        //------------------------
                        cmd += "status_";
                        cmd += names_postables.zeile(i);
                        cmd += ".";
                        cmd += PARAM_STATUS_STATUS;
                        cmd += " AS ";
                        cmd += "Status";
                        cmd += ", ";
                        //------------------------
                    }
                }
            }
            //------------------------
            QString letzte_zwei;
            letzte_zwei += cmd.at(cmd.count()-2);
            letzte_zwei += cmd.at(cmd.count()-1);
            if(letzte_zwei == ", ")
            {
                cmd = cmd.left(cmd.count()-2);
            }
            //------------------------
            //------------------------
            cmd += " FROM ";
            cmd += promat_tabname;
            //------------------------
            /*
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_ARTIKEL;
            cmd += " ON (";
            cmd += promat_tabname;
            cmd += ".";
            cmd += PARAM_PROMAT_ARTIKEL_ID;
            cmd += " = ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            cmd += ")";
            //------------------------
            */
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += " (";
            cmd += TABNAME_ARTIKEL;
            cmd += " LEFT JOIN ";
            cmd += TABNAME_LIEFERANT;
            cmd += " ON ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LIEFERANT;
            cmd += " = ";
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_ID;
            cmd += ")";
            cmd += " ON ";
            cmd += promat_tabname;
            cmd += ".";
            cmd += PARAM_PROMAT_ARTIKEL_ID;
            cmd += " = ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            //------------------------
            for(uint i=1; i<=names_postables.zeilenanzahl() ;i++)
            {
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += names_postables.zeile(i);
                cmd += " ON (";
                cmd += promat_tabname;
                cmd += ".";
                cmd += PARAM_PROMAT_ARTIKEL_ID;
                cmd += " = ";
                cmd += names_postables.zeile(i);
                cmd += ".";
                cmd += PARAM_PROMATPOS_ARTIKEL_ID;
                cmd += ")";
                //------------------------
                //------------------------
                cmd += " LEFT JOIN ";
                cmd += TABNAME_STATUS;
                cmd += " AS ";
                cmd += "status_";
                cmd += names_postables.zeile(i);
                cmd += " ON (";
                cmd += names_postables.zeile(i);
                cmd += ".";
                cmd += PARAM_PROMATPOS_STATUS_ID;
                cmd += " = ";
                cmd += "status_";
                cmd += names_postables.zeile(i);
                cmd += ".";
                cmd += PARAM_STATUS_ID;
                cmd += ")";
                //------------------------
            }
            //------------------------
            cmd += " WHERE ";
            cmd += promat_tabname;
            cmd += ".";
            cmd += PARAM_PROMAT_ME_GESAMTBEDARF;
            cmd += " > 0";

            if(!ui->lineEdit_filter->text().isEmpty())
            {
                cmd += " AND ";
                cmd += "(";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_filter->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_filter->text();
                cmd += "%\'";
                cmd += ")";
            }
            if(statusfilter_is_aktiv == true)
            {
                cmd += " AND ";
                cmd += "(";
                if(ui->checkBox_erfasst->isChecked())
                {
                    cmd += promat_tabname;
                    cmd += ".";
                    cmd += PARAM_PROMAT_ME_ERFASST;
                    cmd += " > ";
                    cmd += "0";
                }
                if(ui->checkBox_unklar->isChecked())
                {
                    if(ui->checkBox_erfasst->isChecked())
                    {
                        cmd += " OR ";
                    }
                    cmd += promat_tabname;
                    cmd += ".";
                    cmd += PARAM_PROMAT_ME_UNKLAR;
                    cmd += " > ";
                    cmd += "0";
                }
                if(ui->checkBox_bestellen->isChecked())
                {
                    if(ui->checkBox_unklar->isChecked() || ui->checkBox_erfasst->isChecked())
                    {
                        cmd += " OR ";
                    }
                    cmd += promat_tabname;
                    cmd += ".";
                    cmd += PARAM_PROMAT_ME_ZURBEST;
                    cmd += " > ";
                    cmd += "0";
                }
                cmd += ")";
            }
            //------------------------
            if(q.exec(cmd))
            {
                model->setQuery(q);
                ui->tableView->setModel(model);

                QString msg;
                msg += int_to_qstring(model->rowCount());
                msg += " Artikel:";
                ui->label_filter->setText(msg);
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
    update_statusbar();
}

void Form_matlist::update_statusbar()
{
    //--------------------------------------------------------------------------------------
    text_zeilenweise megesamt = dbeigen->get_data_tz(promat_tabname, PARAM_PROMAT_ME_GESAMTBEDARF);
    text_zeilenweise aid      = dbeigen->get_data_tz(promat_tabname, PARAM_PROMAT_ARTIKEL_ID);
    text_zeilenweise apreise;
    for(uint i=1; i<=aid.zeilenanzahl(); i++)
    {
        QString preis = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_PREIS, aid.zeile(i));
        apreise.zeile_anhaengen(preis);
    }
    double gesamtpreis = 0;
    for(uint i=1; i<=aid.zeilenanzahl(); i++)
    {
        double preis, menge;
        preis = apreise.zeile(i).toDouble();
        menge = megesamt.zeile(i).toDouble();
        gesamtpreis += preis * menge;
    }
    //--------------------------------------------------------------------------------------
    QString posliste;
    posliste  = TABNAME_PROMATPOSLIST;
    posliste += ui->lineEdit_projekt_id->text();
    text_zeilenweise matpos_ids = dbeigen->get_data_tz(posliste, PARAM_PROMATPOSLIST_ID);
    text_zeilenweise names_postables;
    for(uint i=1; i<=matpos_ids.zeilenanzahl() ;i++)
    {
        if(ui->listWidget_matpos->item(i-1)->checkState() == Qt::Checked)
        {
            QString tabname;
            tabname += TABNAME_PROMATPOS;
            tabname += ui->lineEdit_projekt_id->text();
            tabname += "_";
            tabname += matpos_ids.zeile(i);
            names_postables.zeile_anhaengen(tabname);
        }
    }
    artikel_mengenerfassung ame;
    //Aktuelle Mengen berechnen:
    for(uint i=1; i<=names_postables.zeilenanzahl() ;i++)
    {
        text_zeilenweise artikel_ids = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                            PARAM_PROMATPOS_ARTIKEL_ID);
        text_zeilenweise mengen = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                       PARAM_PROMATPOS_MENGE);
        text_zeilenweise status_ids = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                            PARAM_PROMATPOS_STATUS_ID);
        text_zeilenweise beziehungen = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                            PARAM_PROMATPOS_BEZIEHUNG);
        if(artikel_ids.zeilenanzahl() != 0)
        {
            ame.add_matpos(artikel_ids,\
                           mengen,\
                           status_ids,\
                           beziehungen);
        }
    }
    text_zeilenweise checked_aids, checked_me;
    checked_aids = ame.get_artikel_ids();
    checked_me = ame.get_mengen_gesamt();
    double checkedpreis = 0;
    for(uint i=1; i<=checked_aids.zeilenanzahl() ;i++)
    {
        double menge, preis;
        menge = checked_me.zeile(i).toDouble();
        preis = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_PREIS, checked_aids.zeile(i)).toDouble();
        checkedpreis += menge * preis;
    }
    //--------------------------------------------------------------------------------------

    QString statustext;
    statustext += "Gesamtwert: ";
    statustext += double_to_qstring_euro(gesamtpreis);
    statustext += "\t";
    statustext += "Auswahl: ";
    statustext += double_to_qstring_euro(checkedpreis);

    ui->lineEdit_statusbar->setText(statustext);
}

void Form_matlist::update_promatpos_mengen(QString pro_id, QString pos_id, double pos_me_vor, double pos_me_nach)
{
    QString promatpos_tabname;
    promatpos_tabname += TABNAME_PROMATPOS;
    promatpos_tabname += pro_id;
    promatpos_tabname += "_";
    promatpos_tabname += pos_id;

    text_zeilenweise ids, mengen;
    ids = dbeigen->get_data_tz(promatpos_tabname, PARAM_PROMATPOS_ID);
    mengen = dbeigen->get_data_tz(promatpos_tabname, PARAM_PROMATPOS_MENGE);

    for(uint i=1; i<=ids.zeilenanzahl() ;i++)
    {
        double artikel_menge_vor = mengen.zeile(i).toDouble();
        double artikel_menge_nach;
        if(artikel_menge_vor > 0  &&  pos_me_vor > 0)
        {
            artikel_menge_nach = artikel_menge_vor / pos_me_vor * pos_me_nach;
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Division durch 0 nicht erlaubt!"));
            mb.exec();
            return; //Diese Stelle darf niemals erreicht werden, dah sonst die Mengen nicht mehr stimmen!!!
            //Die Positionen dürfen nicht mit einer Menge von 0 zugelassen werden übers GUI!!
            //Wenn die Positionsmeng auf 0 gesertzt wird werden auch alle Artikelmengem auf 0 gesetzt
            //und dann ist nicht mehr bekannt, wie viel von den Artikeln pro 1x die pos gebraucht wird
            //da in der Position stehs nur die gesamtmenge gespeichert ist
            //und die einzelmenge bei bedaft (Beim Artikel editieren dialog) aus
            //"gesamtmenge geteilt durch pos_menge" berechnet wird
        }
        mengen.zeile_ersaetzen(i, double_to_qstring(artikel_menge_nach));
    }

    for(uint i=1; i<=ids.zeilenanzahl() ;i++)
    {
        dbeigen->data_edit(promatpos_tabname, PARAM_PROMATPOS_MENGE, mengen.zeile(i), ids.zeile(i));
    }
}

//-------------------------------------Buttons:
void Form_matlist::on_pushButton_projektauswahl_clicked()
{
    if(dbeigen != NULL)
    {
        text_zeilenweise projekte, ids;
        projekte = dbeigen->get_data_tz(TABNAME_PROJEKT, PARAM_PROJEKT_NAME);
        ids = dbeigen->get_data_tz(TABNAME_PROJEKT, PARAM_PROJEKT_ID);

        Dialog_dataselection *d = new Dialog_dataselection(this);
        d->set_data(projekte, ids);
        d->setWindowTitle("Projektauswahl");
        d->set_anz_returnwerte(1);
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_set_project(text_zeilenweise))      );
        d->exec();

        delete d;
    }
}

void Form_matlist::on_pushButton_pos_new_clicked()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        Dialog_promatpos *d = new Dialog_promatpos(this);
        d->setWindowTitle("Materialposition anlegen");
        connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
                this, SLOT(slot_new_matpos(text_zeilenweise))   );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt festlegen!"));
        mb.exec();
    }
}

void Form_matlist::on_pushButton_pos_edit_clicked()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        if(ui->listWidget_matpos->currentRow() != -1)
        {
            QString tabname;
            tabname  = TABNAME_PROMATPOSLIST;
            tabname += ui->lineEdit_projekt_id->text();
            QString idbuffer = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");
            promatposlist_current_id = idbuffer;
            QString blockfromuser_id = dbeigen->get_data_qstring(tabname, \
                                                                 PARAM_PROMATPOSLIST_BLOCK, \
                                                                 idbuffer);
            QString blockfromuser = dbeigen->get_data_qstring(tabname, \
                                                              PARAM_PROMATPOSLIST_BLOCK, idbuffer,\
                                                              TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
            blockfromuser += " ";
            blockfromuser += dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BLOCK, idbuffer,\
                                                       TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);
            if(blockfromuser_id == USER_NOBODY_ID || blockfromuser.isEmpty() )
            {
                Dialog_promatpos *d = new Dialog_promatpos(this);
                d->setWindowTitle("Materialposition bearbeiten");
                QString id = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");

                promatpos_menge_vor = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_MENGE, id).toInt();

                text_zeilenweise data;
                data.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BEZ, id));
                data.zeile_anhaengen(int_to_qstring(promatpos_menge_vor));

                d->set_data(data, id);

                connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
                        this, SLOT(slot_edit_matposlist(text_zeilenweise, QString))   );
                connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_matposlist_unblock()));                
                dbeigen->data_edit(tabname, PARAM_PROMATPOSLIST_BLOCK, user, idbuffer);                

                d->exec();
                delete d;
            }else
            {
                Dialog_yes_no *d = new Dialog_yes_no(this);
                d->setWindowTitle("Datensatz bereits gesperrt");
                QString msg;
                msg += "Dieser Datensatz wurde bereits vom Nutzer \"";
                msg += blockfromuser;
                msg += "\" gesperrt.\nBitte stimmen Sie sich mit dem Benutzer ab!\n";
                msg += "Soll dieser trotzdem zum Bearbeiten geoeffnet werden?";
                d->setup(msg);
                connect(d, SIGNAL(signal_yes()), this, SLOT(slot_edit_matposlist_with_block()));

                d->exec();
                delete d;
            }
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Bitte zuerst eine Position wälen!"));
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt festlegen!"));
        mb.exec();
    }
}

void Form_matlist::on_pushButton_pos_edit_rumpf_clicked()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        if(ui->listWidget_matpos->currentRow() != -1)
        {
            QString tabname;
            tabname  = TABNAME_PROMATPOSLIST;
            tabname += ui->lineEdit_projekt_id->text();
            QString idbuffer = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");
            tabname += "_";
            tabname += idbuffer;
            Dialog_promatposrumpf *d = new Dialog_promatposrumpf(this);
            d->set_userid(user);
            d->set_db(dbeigen);
            d->set_tabname(tabname);
            connect(d, SIGNAL(signal_close()),          \
                    this, SLOT(slot_update_table())     );
            d->exec();
            delete d;
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Bitte zuerst eine Position wälen!"));
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt festlegen!"));
        mb.exec();
    }
}

void Form_matlist::on_pushButton_pos_delete_clicked()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        if(ui->listWidget_matpos->currentRow() != -1)
        {
            QString posbez = ui->listWidget_matpos->currentItem()->text();
            posbez = text_rechts(posbez, " ||| ");
            Dialog_yes_no *d = new Dialog_yes_no(this);
            d->setWindowTitle("Materialposition loeschen");
            QString msg;
            msg += "Wollen Sie die Position \"";
            msg += posbez;
            msg += "\" wirklich dauerhaft loeschen?";
            d->setup(msg);
            connect(d, SIGNAL(signal_yes()), this, SLOT(slot_delete_matpos()));
            d->exec();
            delete d;
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Bitte zuerst eine Position wälen!"));
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt festlegen!"));
        mb.exec();
    }
}

void Form_matlist::on_pushButton_check_all_pos_clicked()
{
    for(int i=1; i<=ui->listWidget_matpos->count() ;i++)
    {
        ui->listWidget_matpos->item(i-1)->setCheckState(Qt::Checked);
    }
    update_table();
}

void Form_matlist::on_pushButton_check_activ_pos_clicked()
{
    if(ui->listWidget_matpos->currentRow() != -1)
    {
        for(int i=1; i<=ui->listWidget_matpos->count() ;i++)
        {
            ui->listWidget_matpos->item(i-1)->setCheckState(Qt::Unchecked);
        }
        ui->listWidget_matpos->currentItem()->setCheckState(Qt::Checked);
    }
    update_table();
}

void Form_matlist::on_pushButton_update_table_clicked()
{
    update_table();
}

void Form_matlist::on_pushButton_pos_copy_clicked()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        if(ui->listWidget_matpos->currentRow() != -1)
        {
            QString tabnameposlist;
            tabnameposlist  = TABNAME_PROMATPOSLIST;
            tabnameposlist += ui->lineEdit_projekt_id->text();

            QString id_promatposlist = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");
            QString bez, menge;
            bez = dbeigen->get_data_qstring(tabnameposlist, PARAM_PROMATPOSLIST_BEZ, id_promatposlist);
            bez += "_Kopie";
            menge = dbeigen->get_data_qstring(tabnameposlist, PARAM_PROMATPOSLIST_MENGE, id_promatposlist);

            //Eintrag in promatposlist-Tabelle anlegen:
            create_table_promatpos(bez, menge);

            //Tabelleninhalt kopieren:
            QString tabname = tabnameposlist;
            tabname += "_";
            tabname += id_promatposlist;

            //Inhalt der altenTabelle auslesen:
            text_zeilenweise artikel_ids, mengen, status_ids, beziehungen;
            artikel_ids     = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_ARTIKEL_ID);
            mengen          = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_MENGE);
            status_ids      = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_STATUS_ID);
            beziehungen     = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_BEZIEHUNG);

            //Inhalt in neue Tabelle schreiben:
            text_zeilenweise pa, val;
            pa.zeile_anhaengen(PARAM_PROMATPOS_ARTIKEL_ID);
            pa.zeile_anhaengen(PARAM_PROMATPOS_MENGE);
            pa.zeile_anhaengen(PARAM_PROMATPOS_STATUS_ID);
            pa.zeile_anhaengen(PARAM_PROMATPOS_ERSTELLER);
            pa.zeile_anhaengen(PARAM_PROMATPOS_BEZIEHUNG);

            tabname  = tabnameposlist;
            tabname += "_";
            tabname += dbeigen->get_highest_id(tabnameposlist);

            for(uint i=1; i<=artikel_ids.zeilenanzahl() ;i++)
            {
                val.clear();
                val.zeile_anhaengen(artikel_ids.zeile(i));
                val.zeile_anhaengen(mengen.zeile(i));
                val.zeile_anhaengen(status_ids.zeile(i));
                val.zeile_anhaengen(user);
                val.zeile_anhaengen(beziehungen.zeile(i));
                dbeigen->data_new(tabname, pa, val);
            }

            update_listwidget_matpos();
            slot_update_table();
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Bitte zuerst eine Position wälen!"));
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt festlegen!"));
        mb.exec();
    }
}

void Form_matlist::on_pushButton_pos_import_clicked()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        text_zeilenweise projekte, ids;
        projekte = dbeigen->get_data_tz(TABNAME_PROJEKT, PARAM_PROJEKT_NAME);
        ids = dbeigen->get_data_tz(TABNAME_PROJEKT, PARAM_PROJEKT_ID);

        Dialog_dataselection *d = new Dialog_dataselection(this);
        d->set_data(projekte, ids);
        d->setWindowTitle("Impost von Projekt");
        d->set_anz_returnwerte(1);
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)),         \
                this, SLOT(slot_import_get_projekt_id(text_zeilenweise))    );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt festlegen!"));
        mb.exec();
    }
}

void Form_matlist::on_pushButton_reservieren_clicked()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        ui->checkBox_show_pos->setChecked(false);
        ui->checkBox_einzelmengen->setChecked(true);
        ui->checkBox_reserviert->setChecked(true);
        ui->checkBox_verarbeitet->setChecked(true);

        text_zeilenweise promat_ids;
        text_zeilenweise artikel_id;
        text_zeilenweise me_bestellen;      //zum Reservieren vorgemerkte Menge (mit Status Bestellen)
        text_zeilenweise me_reserviert;     //bereits reservierte Menge

        QString tabname;
        tabname += TABNAME_PROMAT;
        tabname += ui->lineEdit_projekt_id->text();

        promat_ids      = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ID);
        artikel_id      = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ARTIKEL_ID);
        me_bestellen    = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ME_ZURBEST);
        me_reserviert   = dbeigen->get_data_tz(tabname, PARAM_PROMAT_ME_RESERVIERT);

        for(uint i=1; i<=artikel_id.zeilenanzahl() ;i++)
        {
            int vorgemerkt = me_bestellen.zeile(i).toInt(); //vorgemerkte Menge
            int reserviert = me_reserviert.zeile(i).toInt();//beriets reservierte Menge

            int art_resme  = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                                       artikel_id.zeile(i)).toInt();

            if(vorgemerkt > reserviert)
            {
                int zures = vorgemerkt - reserviert; //Bereits reservierte Mengen nicht erneut reservieren!

                //Reservierte Menge im Artikel erhöhen:
                int neue_menge = art_resme + zures;
                dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                   int_to_qstring(neue_menge), artikel_id.zeile(i));

                //Reservierte Menge in Tabelle promat_id erhöhen:
                neue_menge = vorgemerkt;
                dbeigen->data_edit(tabname, PARAM_PROMAT_ME_RESERVIERT, \
                                   int_to_qstring(neue_menge), promat_ids.zeile(i));

            }else if(vorgemerkt < reserviert)
            {
                //art_resme == reservierte Menge im Artikel
                int zustor = reserviert - vorgemerkt;
                if(art_resme >= zustor)
                {
                    //restervierung stornieren:
                    int neue_res_me_promat = vorgemerkt;
                    int neue_res_me_artikel = art_resme - zustor;
                    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                       int_to_qstring(neue_res_me_artikel), artikel_id.zeile(i));
                    dbeigen->data_edit(tabname, PARAM_PROMAT_ME_RESERVIERT, \
                                       int_to_qstring(neue_res_me_promat), promat_ids.zeile(i));
                }else
                {
                    //Teilmenge der restervierung stornieren:
                    int neue_res_me_promat =  vorgemerkt + zustor - art_resme;
                    int neue_res_me_artikel = 0;
                    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                       int_to_qstring(neue_res_me_artikel), artikel_id.zeile(i));
                    dbeigen->data_edit(tabname, PARAM_PROMAT_ME_RESERVIERT, \
                                       int_to_qstring(neue_res_me_promat), promat_ids.zeile(i));
                }
            }
            {
                //Freie Menge berechnen == welche Stückzahl vom Atrikel ist noch nicht verplant:
                int lagerbestand    = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, \
                                                                artikel_id.zeile(i)).toInt();
                int in_bestellung   = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTELLT, \
                                                                artikel_id.zeile(i)).toInt();
                int reserviert      = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_RESERVIERT, \
                                                                artikel_id.zeile(i)).toInt();
                int frei = lagerbestand + in_bestellung - reserviert;
                dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_FREI, \
                                   int_to_qstring(frei), artikel_id.zeile(i));

                //Bestellvorschlag ermitteln und speichern:
                int mindestbestand  = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERST_MIN, \
                                                                artikel_id.zeile(i)).toInt();
                int ve              = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_VE, \
                                                                artikel_id.zeile(i)).toInt();

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
                                       int_to_qstring(bestellvorschlag), artikel_id.zeile(i));
                }else
                {
                    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BESTVOR, \
                                       "0", artikel_id.zeile(i));
                }
            }
        }
        update_table();
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt festlegen!"));
        mb.exec();
    }
}

void Form_matlist::on_pushButton_packliste_clicked()
{
    //Erstellt eine druckbare Packiste für die ausgewählten Positionen

    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        QString posliste;
        posliste  = TABNAME_PROMATPOSLIST;
        posliste += ui->lineEdit_projekt_id->text();
        text_zeilenweise matpos_ids = dbeigen->get_data_tz(posliste, PARAM_PROMATPOSLIST_ID);
        text_zeilenweise names_postables;
        for(uint i=1; i<=matpos_ids.zeilenanzahl() ;i++)
        {
            if(ui->listWidget_matpos->item(i-1)->checkState() == Qt::Checked)
            {
                QString tabname;
                tabname += TABNAME_PROMATPOS;
                tabname += ui->lineEdit_projekt_id->text();
                tabname += "_";
                tabname += matpos_ids.zeile(i);
                names_postables.zeile_anhaengen(tabname);
            }
        }
        artikel_mengenerfassung ame;
        //Aktuelle Mengen berechnen:
        for(uint i=1; i<=names_postables.zeilenanzahl() ;i++)
        {
            text_zeilenweise artikel_ids = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                                PARAM_PROMATPOS_ARTIKEL_ID);
            text_zeilenweise mengen = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                           PARAM_PROMATPOS_MENGE);
            text_zeilenweise status_ids = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                                PARAM_PROMATPOS_STATUS_ID);
            text_zeilenweise beziehungen = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                                PARAM_PROMATPOS_BEZIEHUNG);
            if(artikel_ids.zeilenanzahl() != 0)
            {
                ame.add_matpos(artikel_ids,\
                               mengen,\
                               status_ids,\
                               beziehungen);
            }
        }
        QString printmsg;
        printmsg += "Packliste ";
        printmsg += ui->lineEdit_projekt->text();
        printmsg += "\n\n";
        text_zeilenweise aids, ameng;
        aids   = ame.get_artikel_ids();
        ameng  = ame.get_mengen_bestellen();
        for(uint i=1; i<=aids.zeilenanzahl() ;i++)
        {
            QString me = ameng.zeile(i);
            if(me.contains("."))
            {
                me = text_links(me, ".");
            }
            printmsg += me;
            printmsg += "x";
            printmsg += "  ";
            printmsg += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LIEFERANT,\
                                                  aids.zeile(i), \
                                                  TABNAME_LIEFERANT, PARAM_LIEFERANT_NAME);
            printmsg += " ";
            printmsg += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_NR, aids.zeile(i));
            printmsg += " ";
            printmsg += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZ, aids.zeile(i));
            if(i != aids.zeilenanzahl())
            {
                printmsg += "\n\n";
            }
        }
        Dialog_printbox *d = new Dialog_printbox(this);
        d->setText(printmsg);
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst ein Projekt wählen!"));
        mb.exec();
    }
}
//-------------------------------------private Slots:
void Form_matlist::on_lineEdit_projekt_id_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        //Die Tabelle "promatpos_***" anlegen wenn es sie noch nicht gibt
        // *** == Wert von arg1 == ID des Projektes
        create_table_promatposlist();
        update_listwidget_matpos();
        promat_tabname  = TABNAME_PROMAT;
        promat_tabname += arg1;
        update_table();
        emit signal_proidchanged(arg1);
    }
}

void Form_matlist::slot_set_project(text_zeilenweise p)
{
    ui->lineEdit_projekt_id->setText(p.zeile(1));
    ui->lineEdit_projekt->setText(dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_NAME, p.zeile(1)));
}

void Form_matlist::on_lineEdit_filter_textChanged()
{
    update_table();
}

void Form_matlist::on_checkBox_erfasst_toggled()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
}

void Form_matlist::on_checkBox_unklar_toggled()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
}

void Form_matlist::on_checkBox_bestellen_toggled()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
}

void Form_matlist::on_checkBox_pos_status_toggled()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
}

void Form_matlist::on_checkBox_reserviert_toggled()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
}

void Form_matlist::on_checkBox_einzelmengen_toggled()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
}

void Form_matlist::on_checkBox_verarbeitet_toggled()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
}

void Form_matlist::on_checkBox_show_pos_toggled(bool checked)
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
    {
        update_table();
    }
    if(checked == true)
    {
        ui->checkBox_pos_status->setEnabled(true);
    }else
    {
        ui->checkBox_pos_status->setDisabled(true);
        ui->checkBox_pos_status->setChecked(false);
    }
}

//-------------------------------------public Slots:
void Form_matlist::slot_new_matpos(text_zeilenweise data)
{
    QString bez         = data.zeile(1);
    QString menge       = data.zeile(2);

    create_table_promatpos(bez, menge);
    update_listwidget_matpos();
    update_table();
}

void Form_matlist::slot_edit_matposlist(text_zeilenweise data, QString id)
{
    promatposlist_current_data = data;
    promatposlist_current_id = id;
    slot_edit_matposlist();
}
void Form_matlist::slot_edit_matposlist()
{
    QString tabname;
    tabname  = TABNAME_PROMATPOSLIST;
    tabname += ui->lineEdit_projekt_id->text();
    QString idbuffer = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");

    QString blockfromuser_id = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BLOCK, idbuffer);
    QString blockfromuser = dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROMATPOSLIST_BLOCK, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    blockfromuser += " ";
    blockfromuser += dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BLOCK, idbuffer,\
                                               TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

    QString lasteditinguser = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BEARBEITER, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    lasteditinguser += " ";
    lasteditinguser += dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BEARBEITER, idbuffer,\
                                                 TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

    if(blockfromuser_id != user)
    {
        if(blockfromuser_id == USER_NOBODY_ID)
        {
            QString msg;
            msg += "Die Änderungen konnten nicht gespeichert werden, da der Nutzer \"";
            msg += lasteditinguser;
            msg += "\" zwischenzeitlich den Datensatz bearbeitet hat!";
            QMessageBox mb;
            mb.setText(tr(msg.toStdString().c_str()));
            mb.exec();
        }else
        {
            QString msg;
            msg += "Die Änderungen konnten nicht gespeichert werden, da der Nutzer \"";
            msg += blockfromuser;
            msg += "\" den Datensatz derzeit bearbeitet!";
            QMessageBox mb;
            mb.setText(tr(msg.toStdString().c_str()));
            mb.exec();
        }
    }else
    {
        QString bez         = promatposlist_current_data.zeile(1);
        QString menge       = promatposlist_current_data.zeile(2);

        text_zeilenweise pa, val;

        pa.zeile_anhaengen(PARAM_PROMATPOSLIST_BEZ);
        pa.zeile_anhaengen(PARAM_PROMATPOSLIST_MENGE);
        pa.zeile_anhaengen(PARAM_PROMATPOSLIST_BEARBEITER);
        pa.zeile_anhaengen(PARAM_PROMATPOSLIST_BLOCK);

        val.zeile_anhaengen(bez);
        val.zeile_anhaengen(menge);
        val.zeile_anhaengen(user);
        val.zeile_anhaengen(USER_NOBODY_ID);

        dbeigen->data_edit(tabname, pa, val, promatposlist_current_id);
    }
    update_listwidget_matpos();
    double pos_menge_vor = promatpos_menge_vor;
    double pos_menge_nach = promatposlist_current_data.zeile(2).toDouble();
    if(pos_menge_vor != pos_menge_nach)
    {
        QString pro_id, pos_id;
        pro_id = ui->lineEdit_projekt_id->text();
        pos_id = idbuffer;
        update_promatpos_mengen(pro_id, pos_id, pos_menge_vor, pos_menge_nach);
        slot_update_table();
    }
}
void Form_matlist::slot_edit_matposlist_with_block()
{
    QString tabname;
    tabname  = TABNAME_PROMATPOSLIST;
    tabname += ui->lineEdit_projekt_id->text();
    QString idbuffer = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");

    Dialog_promatpos *d = new Dialog_promatpos(this);
    d->setWindowTitle("Materialposition bearbeiten");
    QString id = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");

    promatpos_menge_vor = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_MENGE, id).toInt();

    text_zeilenweise data;
    data.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BEZ, id));
    data.zeile_anhaengen(int_to_qstring(promatpos_menge_vor));

    d->set_data(data, id);

    connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
            this, SLOT(slot_edit_matposlist(text_zeilenweise, QString))   );
    connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_matposlist_unblock()));

    dbeigen->data_edit(tabname, PARAM_PROMATPOSLIST_BLOCK, user, idbuffer);
    d->exec();
    delete d;
}
void Form_matlist::slot_edit_matposlist_unblock()
{
    QString tabname;
    tabname  = TABNAME_PROMATPOSLIST;
    tabname += ui->lineEdit_projekt_id->text();
    dbeigen->data_edit(tabname, PARAM_PROMATPOSLIST_BLOCK, \
                       USER_NOBODY_ID, promatposlist_current_id);
}

void Form_matlist::slot_delete_matpos()
{
    QString tabname;
    tabname  = TABNAME_PROMATPOSLIST;
    tabname += ui->lineEdit_projekt_id->text();
    QString idbuffer = text_links(ui->listWidget_matpos->currentItem()->text(), " ||| ");
    QString blockfromuser_id = dbeigen->get_data_qstring(tabname, \
                                                         PARAM_PROMATPOSLIST_BLOCK, \
                                                         idbuffer);
    QString blockfromuser = dbeigen->get_data_qstring(tabname, \
                                                      PARAM_PROMATPOSLIST_BLOCK, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    blockfromuser += " ";
    blockfromuser += dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BLOCK, idbuffer,\
                                               TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);
    if(blockfromuser_id == USER_NOBODY_ID || blockfromuser.isEmpty() )
    {
        QString tabname_matpos = tabname;
        tabname_matpos += "_";
        tabname_matpos += idbuffer;

        //Prüfen, ob ein Datensatz innerhalb der matpos-Tabelle blockiert ist:
        bool ok = true;
        text_zeilenweise block = dbeigen->get_data_tz(tabname_matpos, PARAM_PROMATPOS_BLOCK);
        for(uint i=1; i<=block.zeilenanzahl() ;i++)
        {
            QString zeile = block.zeile(i);
            if(  zeile != USER_NOBODY_ID    &&\
                 !zeile.isEmpty()           &&\
                 zeile != "---"                 )
            {
                ok = false;
                break;
            }
        }
        if(ok == true)
        {
            //Eintrag in matposlist löschen:
            dbeigen->data_del(tabname, idbuffer);
            //matpos-Tabelle löschen:

            dbeigen->table_del(tabname_matpos);
            //GUI aktualisieren:
            update_listwidget_matpos();
            slot_update_table();
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Mindestens ein Datensatz in der Materialposition ist durch einen Benutzer blockiert! Position kann nicht gelöscht werden"));
            mb.exec();
        }
    }else
    {
        QString msg;
        msg += "Der Datensetz ist derzeit durch den Nutzer \"";
        msg += blockfromuser;
        msg += "\" gesperrt und kann nicht gelöscht werden!";
        QMessageBox mb;
        mb.setText(tr(msg.toStdString().c_str()));
        mb.exec();
    }

}

void Form_matlist::slot_update_table()
{
    QString posliste;
    posliste  = TABNAME_PROMATPOSLIST;
    posliste += ui->lineEdit_projekt_id->text();

    QString promat_id_tabname;
    promat_id_tabname += TABNAME_PROMAT;
    promat_id_tabname += ui->lineEdit_projekt_id->text();

    text_zeilenweise matposlist_ids = dbeigen->get_data_tz(posliste, PARAM_PROMATPOSLIST_ID);
    text_zeilenweise names_postables;
    for(uint i=1; i<=matposlist_ids.zeilenanzahl() ;i++)
    {
        QString name;
        name += TABNAME_PROMATPOS;
        name += ui->lineEdit_projekt_id->text();
        name += "_";
        name += matposlist_ids.zeile(i);
        names_postables.zeile_anhaengen(name);
    }

    artikel_mengenerfassung ame;
    //Aktuelle Mengen berechnen:
    for(uint i=1; i<=names_postables.zeilenanzahl() ;i++)
    {
        text_zeilenweise artikel_ids = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                            PARAM_PROMATPOS_ARTIKEL_ID);
        text_zeilenweise mengen = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                       PARAM_PROMATPOS_MENGE);
        text_zeilenweise status_ids = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                            PARAM_PROMATPOS_STATUS_ID);
        text_zeilenweise beziehungen = dbeigen->get_data_tz(names_postables.zeile(i), \
                                                            PARAM_PROMATPOS_BEZIEHUNG);
        if(artikel_ids.zeilenanzahl() != 0)
        {
            ame.add_matpos(artikel_ids,\
                           mengen,\
                           status_ids,\
                           beziehungen);
        }
    }

    //Mengen aller erfassten Artikel nullen:
    {
        text_zeilenweise ids = dbeigen->get_data_tz(promat_id_tabname, PARAM_PROMAT_ID);
        text_zeilenweise pa, val;
        pa.zeile_anhaengen(PARAM_PROMAT_ME_GESAMTBEDARF);
        pa.zeile_anhaengen(PARAM_PROMAT_ME_ERFASST);
        pa.zeile_anhaengen(PARAM_PROMAT_ME_UNKLAR);
        pa.zeile_anhaengen(PARAM_PROMAT_ME_ZURBEST);
        val.zeile_anhaengen("0");
        val.zeile_anhaengen("0");
        val.zeile_anhaengen("0");
        val.zeile_anhaengen("0");

        for(uint i=1; i<=ids.zeilenanzahl() ;i++)
        {
            dbeigen->data_edit(promat_id_tabname, pa, val, ids.zeile(i));
        }
    }
    //Daten von ame zurück in Tabelle "promat_id" speichern
    text_zeilenweise artikel_ids    = ame.get_artikel_ids();
    text_zeilenweise menge_gesamt   = ame.get_mengen_gesamt();
    text_zeilenweise menge_erfasst  = ame.get_mengen_erfasst();
    text_zeilenweise menge_unklar   = ame.get_mengen_unklar();
    text_zeilenweise menge_bestellen= ame.get_mengen_bestellen();    

    for(uint i=1; i<=artikel_ids.zeilenanzahl() ;i++)
    {
        QString akt_artikel_id = artikel_ids.zeile(i);
        QString index = dbeigen->get_data_id_QString(promat_id_tabname, PARAM_PROMAT_ARTIKEL_ID, \
                                                     akt_artikel_id);        
        if(index.isEmpty())
        {
            dbeigen->data_new(promat_id_tabname);
            index = dbeigen->get_highest_id(promat_id_tabname);
            dbeigen->data_edit(promat_id_tabname, PARAM_PROMAT_ARTIKEL_ID, akt_artikel_id, index);
        }
        dbeigen->data_edit(promat_id_tabname, PARAM_PROMAT_ME_GESAMTBEDARF,\
                           menge_gesamt.zeile(i), index);
        dbeigen->data_edit(promat_id_tabname, PARAM_PROMAT_ME_ERFASST,\
                           menge_erfasst.zeile(i), index);
        dbeigen->data_edit(promat_id_tabname, PARAM_PROMAT_ME_UNKLAR,\
                           menge_unklar.zeile(i), index);
        dbeigen->data_edit(promat_id_tabname, PARAM_PROMAT_ME_ZURBEST,\
                           menge_bestellen.zeile(i), index);
    }

    /*  Artikel die einmal erfasst wurden dürfen nicht wieder aus der Liste genommen werden,
     * weil es sonst zu falschen Werten in den Funktionen beim Reservieren bzw Stornieren und
     * beim Ändern der Menge einer Position (Menge 0 zu Menge x) kommt!
    //Zeilen mit Artikeln löschen bei denen alle Mengenangaben 0 sind
    {
        text_zeilenweise ids = dbeigen->get_data_tz(promat_id_tabname, PARAM_PROMAT_ID);
        text_zeilenweise menge_verarbeitet;
        menge_gesamt        = dbeigen->get_data_tz(promat_id_tabname, PARAM_PROMAT_ME_GESAMTBEDARF);
        menge_verarbeitet   = dbeigen->get_data_tz(promat_id_tabname, PARAM_PROMAT_ME_VERARBEITET);

        for(uint i=1; i<=ids.zeilenanzahl() ;i++)
        {
            double summe = menge_gesamt.zeile(i).toDouble() + menge_verarbeitet.zeile(i).toDouble();
            if(summe == 0)
            {
                dbeigen->data_del(promat_id_tabname, ids.zeile(i));
            }
        }
    }
    */

    update_table();
}

void Form_matlist::slot_import_get_projekt_id(text_zeilenweise ids)
{
    import_projekt_id = ids.zeile(1);
    //Positionen des Projektes finden:
    QString tabnameposlist;
    tabnameposlist  = TABNAME_PROMATPOSLIST;
    tabnameposlist += import_projekt_id;

    text_zeilenweise pos_ids, pos_bez;
    pos_ids = dbeigen->get_data_tz(tabnameposlist, PARAM_PROMATPOSLIST_ID);
    pos_bez = dbeigen->get_data_tz(tabnameposlist, PARAM_PROMATPOSLIST_BEZ);

    Dialog_dataselection *d = new Dialog_dataselection(this);
    d->set_data(pos_bez, pos_ids);
    d->setWindowTitle("Impost von Projektposition");
    d->set_anz_returnwerte(1);
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)),         \
            this, SLOT(slot_import_get_pos_id(text_zeilenweise))    );
    d->exec();
    delete d;
}

void Form_matlist::slot_import_get_pos_id(text_zeilenweise ids)
{
    QString pos_id = ids.zeile(1);
    QString tabnameposlist;
    tabnameposlist  = TABNAME_PROMATPOSLIST;
    tabnameposlist += import_projekt_id;
    QString bez, menge;
    bez = dbeigen->get_data_qstring(tabnameposlist, PARAM_PROMATPOSLIST_BEZ, pos_id);
    bez += "_Kopie";
    menge = dbeigen->get_data_qstring(tabnameposlist, PARAM_PROMATPOSLIST_MENGE, pos_id);

    //Eintrag in promatposlist-Tabelle anlegen:
    create_table_promatpos(bez, menge);

    //Tabelleninhalt aus dem alten Projekt kopieren:
    QString tabname = tabnameposlist;
    tabname += "_";
    tabname += pos_id;

    //Inhalt der altenTabelle auslesen:
    text_zeilenweise artikel_ids, mengen, status_ids, beziehungen;
    artikel_ids     = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_ARTIKEL_ID);
    mengen          = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_MENGE);
    status_ids      = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_STATUS_ID);
    beziehungen     = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_BEZIEHUNG);

    //Inhalt in neue Tabelle schreiben:
    text_zeilenweise pa, val;
    pa.zeile_anhaengen(PARAM_PROMATPOS_ARTIKEL_ID);
    pa.zeile_anhaengen(PARAM_PROMATPOS_MENGE);
    pa.zeile_anhaengen(PARAM_PROMATPOS_STATUS_ID);
    pa.zeile_anhaengen(PARAM_PROMATPOS_ERSTELLER);
    pa.zeile_anhaengen(PARAM_PROMATPOS_BEZIEHUNG);

    tabnameposlist  = TABNAME_PROMATPOSLIST;
    tabnameposlist += ui->lineEdit_projekt_id->text();

    tabname  = tabnameposlist;
    tabname += "_";
    tabname += dbeigen->get_highest_id(tabnameposlist);

    for(uint i=1; i<=artikel_ids.zeilenanzahl() ;i++)
    {
        val.clear();
        val.zeile_anhaengen(artikel_ids.zeile(i));
        val.zeile_anhaengen(mengen.zeile(i));
        val.zeile_anhaengen(status_ids.zeile(i));
        val.zeile_anhaengen(user);
        val.zeile_anhaengen(beziehungen.zeile(i));
        dbeigen->data_new(tabname, pa, val);
    }

    update_listwidget_matpos();
    slot_update_table();
}





































