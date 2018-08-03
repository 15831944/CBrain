#include "form_artikel.h"
#include "ui_form_artikel.h"

Form_artikel::Form_artikel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_artikel)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_artikel::~Form_artikel()
{
    delete ui;
    delete model;
}

void Form_artikel::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->geometry().height();
    int breite = this->geometry().width();

    //links Buttons 1. Reihe:
    ui->pushButton_new->move(1,1);
    ui->pushButton_new->setFixedWidth(breite/9);

    ui->pushButton_del->move(1 + ui->pushButton_new->geometry().width() + 1\
                             ,1);
    ui->pushButton_del->setFixedWidth(ui->pushButton_new->geometry().width());

    ui->pushButton_edit->move(1 + (ui->pushButton_new->geometry().width() + 1)*2\
                             ,1);
    ui->pushButton_edit->setFixedWidth(ui->pushButton_new->geometry().width());
    ui->pushButton_dupli->move(1 + (ui->pushButton_new->geometry().width() + 1)*3\
                             ,1);
    ui->pushButton_dupli->setFixedWidth(ui->pushButton_new->geometry().width());

    //links Buttons 1. Reihe:
    ui->pushButton_fav_order->setFixedWidth(breite/9*2);
    ui->pushButton_fav_order->move(1,\
                                   1 + ui->pushButton_new->geometry().height() + 1);

    //rechts Suchleiste:
    ui->lineEdit_suche->setFixedHeight(ui->pushButton_new->geometry().height());
    ui->lineEdit_suche->setFixedWidth(breite/3);
    ui->lineEdit_suche->move(breite - 1 - ui->lineEdit_suche->geometry().width()\
                             ,1);

    ui->label_suche->setFixedHeight(ui->pushButton_new->geometry().height());
    ui->label_suche->move(breite - 1 -ui->lineEdit_suche->geometry().width() - 1\
                          - ui->label_suche->geometry().width()\
                          ,1);

    //rechts checkbox nur Favoriten:
    ui->checkBox_only_favorit->setFixedWidth(ui->lineEdit_suche->width());
    ui->checkBox_only_favorit->setFixedHeight(ui->pushButton_new->height());
    ui->checkBox_only_favorit->move(ui->lineEdit_suche->pos().x(),\
                                    1 + ui->pushButton_new->geometry().height() + 1);


    //Tabelle:
    ui->tableView->move(1,\
                        1 + (ui->pushButton_new->geometry().height() + 1)*2);
    ui->tableView->setFixedWidth(breite -2);
    ui->tableView->setFixedHeight(hoehe - ui->tableView->pos().ry() -1);

    QWidget::resizeEvent(event);
}

void Form_artikel::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_artikel::set_user(QString u)
{
    user = u;
}

void Form_artikel::set_user(users *users)
{
    u = users;
}

void Form_artikel::show()
{
    update_table();
    if(u != NULL)
    {
        if(u->function_artfavsort())
        {
            ui->pushButton_fav_order->setEnabled(true);
        }else
        {
            ui->pushButton_fav_order->setDisabled(true);
        }
    }
    setVisible(true);
}

void Form_artikel::update_table()
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
            cmd += PARAM_ARTIKEL_LAGERSTAND;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_PREIS;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LAGERORT;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_VE;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_KOMENT;
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
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_DATERST;
            cmd += ", ";
            //------------------------
            //cmd += TABNAME_PERSONAL;
            cmd += "B";
            cmd += ".";
            cmd += PARAM_PERSONAL_NACHNAME;
            cmd += " AS ";
            cmd += "Bearbeiter";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_DATBEARB;
            //cmd += ", ";
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
            cmd += " LEFT JOIN ";
            cmd += TABNAME_PERSONAL;
            cmd += " AS ";
            cmd += "E";
            cmd += " ON (";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ERSTELLER;
            cmd += " = ";
            //cmd += TABNAME_PERSONAL;
            cmd += "E";
            cmd += ".";
            cmd += PARAM_PERSONAL_ID;
            cmd += ")";
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_PERSONAL;
            cmd += " AS ";
            cmd += "B";
            cmd += " ON (";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BEARBEITER;
            cmd += " = ";
            //cmd += TABNAME_PERSONAL;
            cmd += "B";
            cmd += ".";
            cmd += PARAM_PERSONAL_ID;
            cmd += ")";
            //------------------------
            if(!ui->lineEdit_suche->text().isEmpty() || ui->checkBox_only_favorit->isChecked())
            {
                cmd += " WHERE ";
                if(!ui->lineEdit_suche->text().isEmpty() )
                {
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
                if(!ui->lineEdit_suche->text().isEmpty() && ui->checkBox_only_favorit->isChecked())
                {
                    cmd += " AND ";
                }
                if(ui->checkBox_only_favorit->isChecked())
                {
                    cmd += PARAM_ARTIKEL_ISFAVORIT;
                    cmd += " LIKE 1";
                }
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            //------------------------
            if(!ui->checkBox_only_favorit->isChecked())
            {
                cmd += " ORDER BY ";            //Sortiert nach:
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
            }else
            {
                cmd += " ORDER BY ";            //Sortiert nach:
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_FAVORDER;
            }
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

void Form_artikel::on_lineEdit_suche_textChanged()
{
    update_table();
}

text_zeilenweise Form_artikel::data_for_dialog_artikel(QString id)
{
    text_zeilenweise artikel;

    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_NR, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZ, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LIEFERANT, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERORT, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_VE, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_KOMENT, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZIEHUNG, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_PREIS, id));
    artikel.zeile_anhaengen(dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_ISFAVORIT, id));

    return artikel;
}

//------------------------------------Buttons:
void Form_artikel::on_pushButton_new_clicked()
{
    Dialog_artikel *d = new Dialog_artikel(this);
    d->set_db(dbeigen);
    d->setup();
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_new(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_artikel::on_pushButton_del_clicked()
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
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }

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
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }
    //-------------------------------------------
    Dialog_dataselection *d = new Dialog_dataselection(this);
    d->set_data(tz, ids);
    d->setWindowTitle("Artikel loeschen");
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
            this, SLOT(slot_delete(text_zeilenweise))           );
    d->exec();
    delete d;
}

void Form_artikel::on_pushButton_edit_clicked()
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
            if(!ui->lineEdit_suche->text().isEmpty() || ui->checkBox_only_favorit->isChecked())
            {
                cmd += " WHERE ";
                if(!ui->lineEdit_suche->text().isEmpty() )
                {
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
                if(!ui->lineEdit_suche->text().isEmpty() && ui->checkBox_only_favorit->isChecked())
                {
                    cmd += " AND ";
                }
                if(ui->checkBox_only_favorit->isChecked())
                {
                    cmd += PARAM_ARTIKEL_ISFAVORIT;
                    cmd += " LIKE 1";
                }
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            //------------------------
            if(!ui->checkBox_only_favorit->isChecked())
            {
                cmd += " ORDER BY ";            //Sortiert nach:
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
            }else
            {
                cmd += " ORDER BY ";            //Sortiert nach:
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_FAVORDER;
            }
            //------------------------

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
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }
    //-------------------------------------------
    if(tz.zeilenanzahl() == 1)
    {
        slot_edit_dialog(ids);
    }else
    {
        Dialog_dataselection *d = new Dialog_dataselection(this);
        d->set_data(tz, ids);
        d->setWindowTitle("Artikel bearbeiten (nur einen)");
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_edit_dialog(text_zeilenweise))      );
        d->exec();
        delete d;
    }
}

void Form_artikel::on_pushButton_dupli_clicked()
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
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
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
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }
    //-------------------------------------------
    if(tz.zeilenanzahl() == 1)
    {
        slot_dupli(ids);
    }else
    {
        Dialog_dataselection *d = new Dialog_dataselection(this);
        d->set_data(tz, ids);
        d->setWindowTitle("Artikel bearbeiten (nur einen)");
        d->set_anz_returnwerte(1);
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_dupli(text_zeilenweise))            );
        d->exec();
        delete d;
    }
}

void Form_artikel::on_pushButton_fav_order_clicked()
{
    Dialog_artikel_fav_reihenfolge *d = new Dialog_artikel_fav_reihenfolge(this);
    d->setWindowTitle(tr("Reihenfolge der Favoriten bearbeiten"));
    d->set_db(dbeigen);
    d->exec();
    delete d;
}
//------------------------------------slots:
void Form_artikel::slot_new(text_zeilenweise data)
{
    text_zeilenweise param, values;

    param.zeile_anhaengen(PARAM_ARTIKEL_NR);
    param.zeile_anhaengen(PARAM_ARTIKEL_BEZ);
    param.zeile_anhaengen(PARAM_ARTIKEL_LIEFERANT);
    param.zeile_anhaengen(PARAM_ARTIKEL_LAGERSTAND);
    param.zeile_anhaengen(PARAM_ARTIKEL_ERSTELLER);
    param.zeile_anhaengen(PARAM_ARTIKEL_DATERST);
    param.zeile_anhaengen(PARAM_ARTIKEL_LAGERORT);
    param.zeile_anhaengen(PARAM_ARTIKEL_VE);
    param.zeile_anhaengen(PARAM_ARTIKEL_KOMENT);
    param.zeile_anhaengen(PARAM_ARTIKEL_BEZIEHUNG);
    param.zeile_anhaengen(PARAM_ARTIKEL_PREIS);
    param.zeile_anhaengen(PARAM_ARTIKEL_ISFAVORIT);

    values.zeile_anhaengen(data.zeile(1));
    values.zeile_anhaengen(data.zeile(2));
    values.zeile_anhaengen(data.zeile(3));
    values.zeile_anhaengen("0");
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d());
    values.zeile_anhaengen(data.zeile(4));
    values.zeile_anhaengen(data.zeile(5));
    values.zeile_anhaengen(data.zeile(6));
    values.zeile_anhaengen(data.zeile(7));
    values.zeile_anhaengen(data.zeile(8));
    values.zeile_anhaengen(data.zeile(9));

    dbeigen->data_new(TABNAME_ARTIKEL, param, values);
    update_table();
}

void Form_artikel::slot_delete(text_zeilenweise ids)
{
    dbeigen->data_del(TABNAME_ARTIKEL, ids);
    update_table();
}

void Form_artikel::slot_edit_dialog(text_zeilenweise ids)
{
    if(ids.zeilenanzahl() == 1)
    {
        idbuffer = ids.zeile(1);

        QString blockfromuser_id = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, idbuffer);
        QString blockfromuser = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, idbuffer,\
                                                          TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
        blockfromuser += " ";
        blockfromuser += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, idbuffer,\
                                                   TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

        if(blockfromuser_id == USER_NOBODY_ID || blockfromuser.isEmpty() )
        {
            text_zeilenweise artikel = data_for_dialog_artikel(idbuffer);

            Dialog_artikel *d = new Dialog_artikel(this);
            d->set_db(dbeigen);
            d->setup();
            d->set_data(artikel, idbuffer);
            connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
                    this, SLOT(slot_edit(text_zeilenweise, QString))          );
            connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_dialog_cancel()));

            dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, user, idbuffer);
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
            idbuffer = ids.zeile(1);
            connect(d, SIGNAL(signal_yes()), this, SLOT(slot_edit_dialog()));

            d->exec();
            delete d;
        }

    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte nur einen Artikel zum Bearbeiten auswälen!"));
        mb.exec();
    }
}

void Form_artikel::slot_edit_dialog()
{
    text_zeilenweise artikel = data_for_dialog_artikel(idbuffer);

    Dialog_artikel *d = new Dialog_artikel(this);
    d->set_db(dbeigen);
    d->setup();
    d->set_data(artikel, idbuffer);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
            this, SLOT(slot_edit(text_zeilenweise, QString))          );
    connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_dialog_cancel()));

    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, user, idbuffer);
    d->exec();
    delete d;
}

void Form_artikel::slot_edit_dialog_cancel()
{
    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, USER_NOBODY_ID, idbuffer);
}

void Form_artikel::slot_edit(text_zeilenweise data, QString id)
{
    //data:
    //  Wert 1 = Artikelnummer
    //  Wert 2 = Bezeichnung
    //  Wert 3 = Lieferant

    QString blockfromuser_id = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, idbuffer);
    QString blockfromuser = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    blockfromuser += " ";
    blockfromuser += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BLOCK, idbuffer,\
                                               TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

    QString lasteditinguser = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEARBEITER, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    lasteditinguser += " ";
    lasteditinguser += dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEARBEITER, idbuffer,\
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
        text_zeilenweise param, values;
        datum today;

        param.zeile_anhaengen(PARAM_ARTIKEL_NR);
        param.zeile_anhaengen(PARAM_ARTIKEL_BEZ);
        param.zeile_anhaengen(PARAM_ARTIKEL_LIEFERANT);
        param.zeile_anhaengen(PARAM_ARTIKEL_BEARBEITER);
        param.zeile_anhaengen(PARAM_ARTIKEL_DATBEARB);
        param.zeile_anhaengen(PARAM_ARTIKEL_BLOCK);
        param.zeile_anhaengen(PARAM_ARTIKEL_LAGERORT);
        param.zeile_anhaengen(PARAM_ARTIKEL_VE);
        param.zeile_anhaengen(PARAM_ARTIKEL_KOMENT);
        param.zeile_anhaengen(PARAM_ARTIKEL_BEZIEHUNG);
        param.zeile_anhaengen(PARAM_ARTIKEL_PREIS);
        param.zeile_anhaengen(PARAM_ARTIKEL_ISFAVORIT);

        values.zeile_anhaengen(data.zeile(1));
        values.zeile_anhaengen(data.zeile(2));
        values.zeile_anhaengen(data.zeile(3));
        values.zeile_anhaengen(user);
        values.zeile_anhaengen(today.get_today_y_m_d());
        values.zeile_anhaengen(USER_NOBODY_ID);
        values.zeile_anhaengen(data.zeile(4));
        values.zeile_anhaengen(data.zeile(5));
        values.zeile_anhaengen(data.zeile(6));
        values.zeile_anhaengen(data.zeile(7));
        values.zeile_anhaengen(data.zeile(8));
        values.zeile_anhaengen(data.zeile(9));

        dbeigen->data_edit(TABNAME_ARTIKEL, param, values, id);
    }
    update_table();
}

void Form_artikel::slot_dupli(text_zeilenweise ids)
{
    text_zeilenweise artikel = data_for_dialog_artikel(ids.zeile(1));
    artikel.zeile_ersaetzen(1, "---");//Artikelnummer
    artikel.zeile_ersaetzen(2, "---");//Bezeichnung

    Dialog_artikel *d = new Dialog_artikel(this);
    d->set_db(dbeigen);
    d->setup();
    d->set_data(artikel);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_new(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_artikel::on_checkBox_only_favorit_toggled()
{
    update_table();
}
//------------------------------------






