#include "form_matlist.h"
#include "ui_form_matlist.h"

Form_matlist::Form_matlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_matlist)
{
    ui->setupUi(this);
    ui->lineEdit_projekt->setText("Projekt?");
}

Form_matlist::~Form_matlist()
{
    delete ui;
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

    //links Tabelle:
    ui->tableView->setFixedSize(b_li,\
                                hoehe - 1 - h_btn - 1 - 1);
    ui->tableView->move(1,\
                        1 + h_btn + 1);

    //rechts Kopfzeile:
    ui->pushButton_check_all_pos->setFixedWidth(b_re/2-2);
    ui->pushButton_check_all_pos->move(1 + b_li + 2,\
                                       1);

    ui->pushButton_check_activ_pos->setFixedWidth(ui->pushButton_check_all_pos->width());
    ui->pushButton_check_activ_pos->move(1 + b_li + 2 + ui->pushButton_check_all_pos->width() + 1,\
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
            mb.setText("Fehler bei Datenbankverbindung!");
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
            mb.setText("Fehler bei Datenbankverbindung!");
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
        itemname += table_bez.zeile(i).replace("#br#", "   ");
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
        mb.setText("Bitte zuerst ein Projekt festlegen!");
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

                text_zeilenweise data;
                data.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BEZ, id));
                data.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_MENGE, id));

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
            mb.setText("Bitte zuerst eine Position waelen!");
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst ein Projekt festlegen!");
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

            //connect....

            d->exec();
            delete d;
        }else
        {
            QMessageBox mb;
            mb.setText("Bitte zuerst eine Position waelen!");
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst ein Projekt festlegen!");
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
            mb.setText("Bitte zuerst eine Position waelen!");
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst ein Projekt festlegen!");
        mb.exec();
    }
}

void Form_matlist::on_pushButton_check_all_pos_clicked()
{
    for(int i=1; i<=ui->listWidget_matpos->count() ;i++)
    {
        ui->listWidget_matpos->item(i-1)->setCheckState(Qt::Checked);
    }
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
    }
}

void Form_matlist::slot_set_project(text_zeilenweise p)
{
    ui->lineEdit_projekt_id->setText(p.zeile(1));
    ui->lineEdit_projekt->setText(dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_NAME, p.zeile(1)));
}

//-------------------------------------public Slots:
void Form_matlist::slot_new_matpos(text_zeilenweise data)
{
    QString bez         = data.zeile(1);
    QString menge       = data.zeile(2);

    create_table_promatpos(bez, menge);
    update_listwidget_matpos();
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
            msg += "Die Aenderungen konnten nicht gespeichert werden, da der Nutzer \"";
            msg += lasteditinguser;
            msg += "\" zwischenzeitlich den Datensatz bearbeitet hat!";
            QMessageBox mb;
            mb.setText(msg);
            mb.exec();
        }else
        {
            QString msg;
            msg += "Die Aenderungen konnten nicht gespeichert werden, da der Nutzer \"";
            msg += blockfromuser;
            msg += "\" den Datensatz derzeit bearbeitet!";
            QMessageBox mb;
            mb.setText(msg);
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

    text_zeilenweise data;
    data.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_BEZ, id));
    data.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOSLIST_MENGE, id));

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
        }else
        {
            QMessageBox mb;
            mb.setText("Mindestens ein Datensatz in der Materialposition ist durch einen Benutzer blockiert! Position kann nicht geloescht werden");
            mb.exec();
        }
    }else
    {
        QString msg;
        msg += "Der Datensetz ist derzeit durch den Nutzer \"";
        msg += blockfromuser;
        msg += "\" gesperrt und kann nicht geloescht werden!";
        QMessageBox mb;
        mb.setText(msg);
        mb.exec();
    }

}
















