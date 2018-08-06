#include "form_matlist.h"
#include "ui_form_matlist.h"

Form_matlist::Form_matlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_matlist)
{
    ui->setupUi(this);
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

void Form_matlist::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->height();
    int breite = this->width();

    int b_li = breite/5*3;
    int b_re = breite - b_li - 4;   //Rand_li = 1 + Rand_re = 1 + Rand_mi = 2 == 4

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

    //links 2. Zeile:
    ui->checkBox_erfasst->setFixedHeight(h_btn);
    ui->checkBox_unklar->setFixedHeight(h_btn);
    ui->checkBox_bestellen->setFixedHeight(h_btn);

    ui->checkBox_erfasst->move(1,\
                               1 + h_btn + 1);
    ui->checkBox_unklar->move(1 + ui->checkBox_erfasst->width() + 1,\
                              1 + h_btn + 1);
    ui->checkBox_bestellen->move(1 + ui->checkBox_erfasst->width() + 1 + ui->checkBox_unklar->width() + 1,\
                                 1 + h_btn + 1);

    //links Tabelle:
    ui->tableView->setFixedSize(b_li,\
                                hoehe - 1 - h_btn*2 - 3 );
    ui->tableView->move(1,\
                        1 + (h_btn + 1)*2);

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

    //rechts Listwidget + Buttons
    ui->listWidget_matpos->setFixedSize(b_re,\
                                        hoehe - 1*5 - h_btn*3);
    ui->listWidget_matpos->move(1 + b_li + 2,\
                                1 + h_btn + 1);

    ui->pushButton_pos_new->setFixedWidth(b_re/3 - 2*2);
    ui->pushButton_pos_delete->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_edit->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_copy->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_import->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_edit_rumpf->setFixedWidth(ui->pushButton_pos_new->width());

    ui->pushButton_pos_new->move(1 + b_li + 2,\
                                 1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_delete->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                    1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_edit->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                  1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);

    ui->pushButton_pos_copy->move(1 + b_li + 2,\
                                  1 + h_btn + 1 + ui->listWidget_matpos->height() + 1 + h_btn + 1);
    ui->pushButton_pos_import->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                    1 + h_btn + 1 + ui->listWidget_matpos->height() + 1 + h_btn + 1);
    ui->pushButton_pos_edit_rumpf->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                        1 + h_btn + 1 + ui->listWidget_matpos->height() + 1 + h_btn + 1);

    QWidget::resizeEvent(event);
}

void Form_matlist::create_table_promatposlist()
{
    //Tabelle die die Materialpositionen erfasst

    if(!ui->lineEdit_projekt_id->text().isEmpty())
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
            cmd += " float";
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
            name += int_to_qstring(i);
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
            cmd += promat_tabname;
            cmd += ".";
            cmd += PARAM_PROMAT_ME_GESAMTBEDARF;
            cmd += ", ";
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
            cmd += promat_tabname;
            cmd += ".";
            cmd += PARAM_PROMAT_ME_VERARBEITET;
            if(names_postables.zeilenanzahl() > 0)
            {
                cmd += ", ";
            }
            //------------------------
            //------------------------
            for(uint i=1; i<=names_postables.zeilenanzahl() ;i++)
            {
                //------------------------
                cmd += names_postables.zeile(i);
                cmd += ".";
                cmd += PARAM_PROMATPOS_MENGE;
                cmd += " AS ";
                cmd += matpos_names.zeile(i);
                cmd += ", ";
                //------------------------
                cmd += "status_";
                cmd += names_postables.zeile(i);
                cmd += ".";
                cmd += PARAM_STATUS_STATUS;
                cmd += " AS ";
                cmd += "Status";
                //cmd += ", ";
                //------------------------
                if(i != names_postables.zeilenanzahl())
                {
                    cmd += ", ";
                }
            }
            //------------------------
            //------------------------
            cmd += " FROM ";
            cmd += promat_tabname;
            //------------------------
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
            if(!ui->lineEdit_filter->text().isEmpty() || statusfilter_is_aktiv == true)
            {
                cmd += " WHERE ";
                if(!ui->lineEdit_filter->text().isEmpty())
                {
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
                if(!ui->lineEdit_filter->text().isEmpty() && statusfilter_is_aktiv == true)
                {
                    cmd += " AND ";
                }
                if(statusfilter_is_aktiv == true)
                {
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
        double artikel_menge_nach = artikel_menge_vor / pos_me_vor * pos_me_nach;
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

void Form_matlist::on_checkBox_erfasst_toggled(bool checked)
{
    update_table();
}

void Form_matlist::on_checkBox_unklar_toggled(bool checked)
{
    update_table();
}

void Form_matlist::on_checkBox_bestellen_toggled(bool checked)
{
    update_table();
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

    update_table();
}























