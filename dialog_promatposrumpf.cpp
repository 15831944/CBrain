#include "dialog_promatposrumpf.h"
#include "ui_dialog_promatposrumpf.h"

Dialog_promatposrumpf::Dialog_promatposrumpf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_promatposrumpf)
{
    ui->setupUi(this);
    current_userid = "0";
    this->model = new QSqlQueryModel();
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
}

Dialog_promatposrumpf::~Dialog_promatposrumpf()
{
    delete ui;
    delete model;
}

void Dialog_promatposrumpf::set_tabname(QString name)
{
    tabname = name;
    this->setWindowTitle(name);
    update_table();
    update_favorit();
}

void Dialog_promatposrumpf::set_userid(QString id)
{
    current_userid = id;
}

void Dialog_promatposrumpf::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Dialog_promatposrumpf::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->height();
    int breite = this->width();

    int b_li = breite/5*3;
    int b_re = breite - b_li - 4;   //Rand_li = 1 + Rand_re = 1 + Rand_mi = 2 == 4

    int h_btn = ui->pushButton_new->height();
    int b_btn_li = (b_li-2)/3;

    //links Kopfzeile:
    ui->pushButton_new->setFixedWidth(b_btn_li);
    ui->pushButton_delete->setFixedWidth(b_btn_li);
    ui->pushButton_edit->setFixedWidth(b_btn_li);

    ui->pushButton_new->move(1,1);
    ui->pushButton_delete->move(1 + b_btn_li + 1 ,1);
    ui->pushButton_edit->move(1 + b_btn_li + 1 + b_btn_li + 1 ,1);

    //links 2. Zeile:
    ui->label_filter->setFixedHeight(ui->lineEdit_filter->height());
    ui->label_filter->move(1, 1 + h_btn + 1);

    ui->lineEdit_filter->setFixedWidth(b_li - 1 - ui->label_filter->width());
    ui->lineEdit_filter->move(1 + ui->label_filter->width() + 1,\
                              1 + h_btn + 1);

    //links tableview:
    ui->tableView->setFixedWidth(b_li);
    ui->tableView->setFixedHeight(hoehe - 4 - h_btn*2 - ui->lineEdit_filter->height());
    ui->tableView->move(1, 1 + h_btn + 1 + ui->lineEdit_filter->height() - 1);

    //links Ferig btn:
    ui->pushButton_close->setFixedWidth(ui->tableView->width());
    ui->pushButton_close->move(1, \
                               1 + h_btn + 1 + ui->lineEdit_filter->height() - 1 \
                               + 1 + ui->tableView->height());

    //rechts Kopfzeile:
    ui->label_favourite->setFixedSize(b_li, h_btn);
    ui->label_favourite->move(1 + b_li + 1, 1);

    //rechts list-Widget:
    ui->listWidget_favourite->setFixedWidth(b_re);
    ui->listWidget_favourite->setFixedHeight(hoehe - 2 - h_btn*2);
    ui->listWidget_favourite->move(1 + b_li + 1, 1 + h_btn + 1);

    //rechts btn unten:
    ui->pushButton_usefavourite->setFixedWidth(b_re-10);
    ui->pushButton_usefavourite->move(1 + b_li + 1, 1 + h_btn + 1 + ui->listWidget_favourite->height() + 1);

    QWidget::resizeEvent(event);
}

void Dialog_promatposrumpf::show()
{
    update_table();
    update_favorit();
    setVisible(true);
}

void Dialog_promatposrumpf::update_table()
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
        cmd += tabname;
        cmd += ".";
        cmd += PARAM_PROMATPOS_ID;
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
        cmd += tabname;
        cmd += ".";
        cmd += PARAM_PROMATPOS_MENGE;
        cmd += ", ";
        //------------------------
        cmd += TABNAME_STATUS;
        cmd += ".";
        cmd += PARAM_STATUS_STATUS;
        //cmd += ", ";
        //------------------------
        cmd += " FROM ";
        cmd += tabname;
        //------------------------
        cmd += " LEFT JOIN ";
        cmd += TABNAME_ARTIKEL;
        cmd += " ON (";
        cmd += tabname;
        cmd += ".";
        cmd += PARAM_PROMATPOS_ARTIKEL_ID;
        cmd += " = ";
        cmd += TABNAME_ARTIKEL;
        cmd += ".";
        cmd += PARAM_ARTIKEL_ID;
        cmd += ")";
        //------------------------
        cmd += " LEFT JOIN ";
        cmd += TABNAME_STATUS;
        cmd += " ON (";
        cmd += tabname;
        cmd += ".";
        cmd += PARAM_PROMATPOS_STATUS_ID;
        cmd += " = ";
        cmd += TABNAME_STATUS;
        cmd += ".";
        cmd += PARAM_STATUS_ID;
        cmd += ")";
        //------------------------
        if(!ui->lineEdit_filter->text().isEmpty())
        {
            cmd += " WHERE ";
            cmd += PARAM_ARTIKEL_NR;
            cmd += " LIKE \'%";
            cmd += ui->lineEdit_filter->text();
            cmd += "%\'";
            cmd += " OR ";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += " LIKE \'%";
            cmd += ui->lineEdit_filter->text();
            cmd += "%\'";
        }
        //------------------------
        cmd += " GROUP BY ";
        cmd += tabname;
        cmd += ".";
        cmd += PARAM_PROMATPOS_ID;
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

void Dialog_promatposrumpf::update_favorit()
{
    if(dbeigen != NULL)
    {
        fav_id_tz.clear();
        fav_nr_tz.clear();
        fav_bez_tz.clear();

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
            //------------------------
            cmd += "SELECT ";
            //------------------------
            cmd += PARAM_ARTIKEL_ID;
            cmd += ", ";
            //------------------------
            cmd += PARAM_ARTIKEL_NR;
            cmd += ", ";
            //------------------------
            cmd += PARAM_ARTIKEL_BEZ;
            //cmd += ", ";
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;
            //------------------------
            cmd += " WHERE ";
            cmd += PARAM_ARTIKEL_ISFAVORIT;
            cmd += " LIKE 1";
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_FAVORDER;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    fav_id_tz.zeile_anhaengen(q.value(0).toString());
                    fav_nr_tz.zeile_anhaengen(q.value(1).toString());
                    fav_bez_tz.zeile_anhaengen(q.value(2).toString());
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

        for(uint i=1; i<=fav_id_tz.zeilenanzahl() ;i++)
        {
            QString zeile;
            zeile += fav_nr_tz.zeile(i);
            zeile += "   ";
            zeile += fav_bez_tz.zeile(i);
            ui->listWidget_favourite->addItem(zeile);
        }
    }
}

//--------------------------------------------private Funktionen:
bool Dialog_promatposrumpf::artikel_bereits_erfasst(QString artikel_id)
{
    if(dbeigen != NULL)
    {
        text_zeilenweise ids = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_ARTIKEL_ID);

        for(uint i=1; i<=ids.zeilenanzahl() ;i++)
        {
            if(ids.zeile(i) == artikel_id)
            {
                return true;
            }
        }
    }

    return false;
}

QString Dialog_promatposrumpf::get_own_id_from_artikel_id(QString artikel_id)
{
    if(dbeigen != NULL)
    {
        text_zeilenweise ids = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_ID);
        text_zeilenweise artikal_ids = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_ARTIKEL_ID);

        for(uint i=1; i<=ids.zeilenanzahl() ;i++)
        {
            if(artikal_ids.zeile(i) == artikel_id)
            {
                return ids.zeile(i);
            }
        }
    }
    return "";
}

void Dialog_promatposrumpf::bearbeiten_dialog(QString artikel_id)
{
    idbuffer = get_own_id_from_artikel_id(artikel_id);
    dbeigen->data_edit(tabname, PARAM_PROMATPOS_BLOCK, current_userid, idbuffer);

    Dialog_artikel_verwenden *d = new Dialog_artikel_verwenden(this);
    d->set_db(dbeigen);
    d->set_artikel_id(artikel_id);

    double menge_gesamt    = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_MENGE, idbuffer).toDouble();
    QString tabname_promatposlist;
    tabname_promatposlist += TABNAME_PROMATPOSLIST;
    tabname_promatposlist += text_mitte(tabname, "_", "_");
    QString promatposlist_index = text_rechts(tabname, "_");
    promatposlist_index = text_rechts(promatposlist_index, "_");
    double menge_position =dbeigen->get_data_qstring(tabname_promatposlist, PARAM_PROMATPOSLIST_MENGE,\
                                                     promatposlist_index).toDouble();
    QString menge_pro_1  = double_to_qstring(menge_gesamt / menge_position);

    text_zeilenweise data_for_dialog;
    data_for_dialog.zeile_anhaengen(menge_pro_1);
    data_for_dialog.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_STATUS_ID, idbuffer));
    data_for_dialog.zeile_anhaengen(dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BEZIEHUNG, idbuffer));
    d->set_data(data_for_dialog);
    d->setWindowTitle(tr("Artikeleintrag bearbeiten"));
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)), \
            this, SLOT(slot_edit_data(text_zeilenweise))    );
    connect(d, SIGNAL(signal_cancel()),             \
            this, SLOT(slot_edit_dialog_cancel())   );
    d->exec();
    delete d;
}
//--------------------------------------------Buttons:
void Dialog_promatposrumpf::on_pushButton_usefavourite_clicked()
{
    if(ui->listWidget_favourite->currentRow() != -1)
    {
        QString artikel_id = fav_id_tz.zeile(ui->listWidget_favourite->currentRow()+1);

        if(!artikel_bereits_erfasst(artikel_id) == true)
        {
            Dialog_artikel_verwenden *d = new Dialog_artikel_verwenden(this);
            d->set_db(dbeigen);
            d->set_artikel_id(artikel_id);
            d->setWindowTitle(tr("Favoriten verwenden"));
            connect(d, SIGNAL(signal_send_data(text_zeilenweise)), \
                    this, SLOT(slot_new_data(text_zeilenweise))    );
            d->exec();
            delete d;
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Artikeleintrag ist bereits in der Liste!"));
            mb.exec();

            //Bearbeiten-Dialog öffnen
            bearbeiten_dialog(artikel_id);
        }

    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuest einen Favoriten wählen"));
        mb.exec();
    }
}

void Dialog_promatposrumpf::on_pushButton_new_clicked()
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
            //cmd += ", ";
            //------------------------
            cmd += " FROM ";
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
            //------------------------

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    ids.zeile_anhaengen(q.value(0).toString()); //ID

                    QString tmp;
                    tmp += q.value(1).toString();   //Artikel-Nummer
                    tmp += "   ";
                    tmp += q.value(2).toString();   //Artikel-Bezeichnung
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
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }
    //-------------------------------------------
    Dialog_dataselection *d = new Dialog_dataselection(this);
    d->set_data(tz, ids);
    d->setWindowTitle("Artikeleintag erstellen");
    d->set_anz_returnwerte(1);
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
            this, SLOT(slot_new_artikel(text_zeilenweise))           );
    d->exec();
    delete d;
}

void Dialog_promatposrumpf::on_pushButton_delete_clicked()
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
            cmd += "SELECT ";
            //------------------------
            cmd += tabname;
            cmd += ".";
            cmd += PARAM_PROMATPOS_ID;
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
            //cmd += ", ";
            //------------------------
            cmd += " FROM ";
            cmd += tabname;
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_ARTIKEL;
            cmd += " ON (";
            cmd += tabname;
            cmd += ".";
            cmd += PARAM_PROMATPOS_ARTIKEL_ID;
            cmd += " = ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            cmd += ")";
            //------------------------
            if(!ui->lineEdit_filter->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_filter->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_filter->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += tabname;
            cmd += ".";
            cmd += PARAM_PROMATPOS_ID;
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
                    ids.zeile_anhaengen(q.value(0).toString()); //ID

                    QString tmp;
                    tmp += q.value(1).toString();   //Artikel-Nummer
                    tmp += "   ";
                    tmp += q.value(2).toString();   //Artikel-Bezeichnung
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
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }
    //-------------------------------------------
    Dialog_dataselection *d = new Dialog_dataselection(this);
    d->set_data(tz, ids);
    d->setWindowTitle("Artikeleintag loeschen");
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
            this, SLOT(slot_delete(text_zeilenweise))           );
    d->exec();
    delete d;
}

void Dialog_promatposrumpf::on_pushButton_edit_clicked()
{
    Dialog_text_input *d = new Dialog_text_input(this);
    d->setWindowTitle("Eintrag bearbeiten");
    d->set_infotext("Bitte ID-Nummer des Listenelementes eingeben");
    connect(d, SIGNAL(signal_userinput(QString)),   \
            this, SLOT(slot_edit_dialog(QString))   );
    d->exec();
    delete d;
}

void Dialog_promatposrumpf::on_pushButton_close_clicked()
{
    this->close();
    emit signal_close();
}

//--------------------------------------------private slots:
void Dialog_promatposrumpf::on_lineEdit_filter_textChanged()
{
    update_table();
}

//--------------------------------------------public slots:
void Dialog_promatposrumpf::slot_new_data(text_zeilenweise data)
{
    if(dbeigen != NULL)
    {
        text_zeilenweise pa, val;

        pa.zeile_anhaengen(PARAM_PROMATPOS_ARTIKEL_ID);
        pa.zeile_anhaengen(PARAM_PROMATPOS_MENGE);
        pa.zeile_anhaengen(PARAM_PROMATPOS_STATUS_ID);
        pa.zeile_anhaengen(PARAM_PROMATPOS_ERSTELLER);
        pa.zeile_anhaengen(PARAM_PROMATPOS_BEZIEHUNG);

        val.zeile_anhaengen(data.zeile(1));
        val.zeile_anhaengen(data.zeile(2));
        val.zeile_anhaengen(data.zeile(3));
        val.zeile_anhaengen(current_userid);
        val.zeile_anhaengen(data.zeile(4));

        dbeigen->data_new(tabname, pa, val);

        update_table();
    }
}

void Dialog_promatposrumpf::slot_edit_data(text_zeilenweise data)
{
    if(dbeigen != NULL)
    {
        QString blockfromuser_id = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BLOCK, idbuffer);
        QString blockfromuser = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BLOCK, idbuffer,\
                                                          TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
        blockfromuser += " ";
        blockfromuser += dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BLOCK, idbuffer,\
                                                   TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

        QString lasteditinguser = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BEARBEITER, idbuffer,\
                                                          TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
        lasteditinguser += " ";
        lasteditinguser += dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BEARBEITER, idbuffer,\
                                                     TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

        if(blockfromuser_id != current_userid)
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

            double menge_pro_1 = data.zeile(2).toDouble();
            QString tabname_promatposlist;
            tabname_promatposlist += TABNAME_PROMATPOSLIST;
            tabname_promatposlist += text_mitte(tabname, "_", "_");
            QString promatposlist_index = text_rechts(tabname, "_");
            promatposlist_index = text_rechts(promatposlist_index, "_");
            double menge_position =dbeigen->get_data_qstring(tabname_promatposlist, PARAM_PROMATPOSLIST_MENGE,\
                                                             promatposlist_index).toDouble();
            double menge_gesamt = menge_pro_1 * menge_position;

            text_zeilenweise pa, val;

            pa.zeile_anhaengen(PARAM_PROMATPOS_MENGE);
            pa.zeile_anhaengen(PARAM_PROMATPOS_STATUS_ID);
            pa.zeile_anhaengen(PARAM_PROMATPOS_BEARBEITER);
            pa.zeile_anhaengen(PARAM_PROMATPOS_BEZIEHUNG);
            pa.zeile_anhaengen(PARAM_PROMATPOS_BLOCK);

            val.zeile_anhaengen(double_to_qstring(menge_gesamt));
            val.zeile_anhaengen(data.zeile(3));
            val.zeile_anhaengen(current_userid);
            val.zeile_anhaengen(data.zeile(4));
            val.zeile_anhaengen(USER_NOBODY_ID);

            QString artikel_id = data.zeile(1);

            dbeigen->data_edit(tabname, pa, val, get_own_id_from_artikel_id(artikel_id));
        }
        update_table();
    }
}

void Dialog_promatposrumpf::slot_delete(text_zeilenweise ids)
{
    dbeigen->data_del(tabname, ids);
    update_table();
}

void Dialog_promatposrumpf::slot_new_artikel(text_zeilenweise artikel_ids)
{
    QString artikel_id = artikel_ids.zeile(1);
    if(!artikel_bereits_erfasst(artikel_id) == true)
    {
        Dialog_artikel_verwenden *d = new Dialog_artikel_verwenden(this);
        d->set_db(dbeigen);
        d->set_artikel_id(artikel_id);
        d->setWindowTitle(tr("Artikel verwenden"));
        connect(d, SIGNAL(signal_send_data(text_zeilenweise)), \
                this, SLOT(slot_new_data(text_zeilenweise))    );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Artikeleintrag ist bereits in der Liste!"));
        mb.exec();

        //Bearbeiten-Dialog öffnen
        bearbeiten_dialog(artikel_id);
    }

}

void Dialog_promatposrumpf::slot_edit_dialog(QString listid)
{
    //Prüfen, ob es ein Eintrag mit dieser ID gibt:
    text_zeilenweise listen_ids = dbeigen->get_data_tz(tabname, PARAM_PROMATPOS_ID);
    bool existiert = false;
    for(uint i=1; i<=listen_ids.zeilenanzahl() ;i++)
    {
        if(listen_ids.zeile(i) == listid)
        {
            existiert = true;
            break;
        }
    }
    if(existiert == true)
    {
        idbuffer = listid;

        QString blockfromuser_id = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BLOCK, idbuffer);
        QString blockfromuser = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, idbuffer,\
                                                          TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
        blockfromuser += " ";
        blockfromuser += dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_BLOCK, idbuffer,\
                                                   TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

        if(blockfromuser_id == USER_NOBODY_ID || blockfromuser.isEmpty() )
        {
            QString artikel_id = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_ARTIKEL_ID, idbuffer);
            bearbeiten_dialog(artikel_id);
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
            idbuffer = listid;
            connect(d, SIGNAL(signal_yes()), this, SLOT(slot_edit_dialog()));

            d->exec();
            delete d;
        }

    }else
    {
        idbuffer.clear();
        QMessageBox mb;
        mb.setText(tr("Ein Eintrag mit dieser ID existiert nicht!"));
        mb.exec();
    }
}

void Dialog_promatposrumpf::slot_edit_dialog()
{
    QString artikel_id = dbeigen->get_data_qstring(tabname, PARAM_PROMATPOS_ARTIKEL_ID, idbuffer);
    bearbeiten_dialog(artikel_id);
}

void Dialog_promatposrumpf::slot_edit_dialog_cancel()
{
    dbeigen->data_edit(tabname, PARAM_PROMATPOS_BLOCK, USER_NOBODY_ID, idbuffer);
}

//--------------------------------------------




























