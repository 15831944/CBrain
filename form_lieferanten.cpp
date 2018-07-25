#include "form_lieferanten.h"
#include "ui_form_lieferanten.h"

Form_lieferanten::Form_lieferanten(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_lieferanten)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_lieferanten::~Form_lieferanten()
{
    delete ui;
    delete model;
}

void Form_lieferanten::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->geometry().height();
    int breite = this->geometry().width();

    //Buttons:
    ui->pushButton_new->move(1,1);
    ui->pushButton_new->setFixedWidth(breite/7);

    ui->pushButton_del->move(1 + ui->pushButton_new->geometry().width() + 1\
                             ,1);
    ui->pushButton_del->setFixedWidth(ui->pushButton_new->geometry().width());

    ui->pushButton_edit->move(1 + (ui->pushButton_new->geometry().width() + 1)*2\
                             ,1);
    ui->pushButton_edit->setFixedWidth(ui->pushButton_new->geometry().width());

    //Suchleiste:
    ui->lineEdit_suche->setFixedHeight(ui->pushButton_new->geometry().height());
    ui->lineEdit_suche->setFixedWidth(breite/3);
    ui->lineEdit_suche->move(breite - 1 - ui->lineEdit_suche->geometry().width()\
                             ,1);

    ui->label_suche->setFixedHeight(ui->pushButton_new->geometry().height());
    ui->label_suche->move(breite - 1 -ui->lineEdit_suche->geometry().width() - 1\
                          - ui->label_suche->geometry().width()\
                          ,1);

    //Tabelle:
    ui->tableView->move(1,\
                        1 + ui->pushButton_new->geometry().height() + 1);
    ui->tableView->setFixedWidth(breite -2);
    ui->tableView->setFixedHeight(hoehe - ui->tableView->pos().ry() -1);

    QWidget::resizeEvent(event);
}

void Form_lieferanten::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_lieferanten::set_user(QString u)
{
    user = u;
}

void Form_lieferanten::show()
{
    update_table();
    setVisible(true);
}

void Form_lieferanten::update_table()
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
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_NAME;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_KOMMENT;
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
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_DATERST;
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
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_DATBEARB;
            //cmd += ", ";
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_LIEFERANT;
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_PERSONAL;
            cmd += " AS ";
            cmd += "E";
            cmd += " ON (";
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_ERSTELLER;
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
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_BEARBEITER;
            cmd += " = ";
            //cmd += TABNAME_PERSONAL;
            cmd += "B";
            cmd += ".";
            cmd += PARAM_PERSONAL_ID;
            cmd += ")";
            //------------------------
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_LIEFERANT_ID;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_LIEFERANT_NAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_ID;
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_NAME;
            //------------------------


            if(q.exec(cmd))
            {
                model->setQuery(q);
                ui->tableView->setModel(model);

                QString msg;
                msg += int_to_qstring(model->rowCount());
                msg += " Lieferanten:";
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
            mb.setText("Fehler bei Datenbankverbindung!");
            mb.exec();
        }
    }
    //-------------------------------------------
}

void Form_lieferanten::on_lineEdit_suche_textChanged()
{
    update_table();
}

//------------------------------------Buttons:
void Form_lieferanten::on_pushButton_new_clicked()
{
    Dialog_lieferanten *d = new Dialog_lieferanten;
    d->setWindowTitle("Lieferant anlegen");
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_new(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_lieferanten::on_pushButton_del_clicked()
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
            cmd += TABNAME_LIEFERANT;
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_LIEFERANT_NAME;
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
                    tmp += q.value(1).toString();   //Name
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
    Dialog_dataselection *d = new Dialog_dataselection;
    d->set_data(tz, ids);
    d->setWindowTitle("Lieferant loeschen");
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
            this, SLOT(slot_delete(text_zeilenweise))           );
    d->exec();
    delete d;
}

void Form_lieferanten::on_pushButton_edit_clicked()
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
            cmd += TABNAME_LIEFERANT;
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_LIEFERANT_NAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_ID;
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_LIEFERANT;
            cmd += ".";
            cmd += PARAM_LIEFERANT_NAME;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    ids.zeile_anhaengen(q.value(0).toString()); //ID

                    QString tmp;
                    tmp += q.value(1).toString();   //Name
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
    if(tz.zeilenanzahl() == 1)
    {
        slot_edit_dialog(ids);
    }else
    {
        Dialog_dataselection *d = new Dialog_dataselection;
        d->set_data(tz, ids);
        d->setWindowTitle("Lieferant bearbeiten (nur einen)");
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_edit_dialog(text_zeilenweise))      );
        d->exec();
        delete d;
    }
}

//------------------------------------slots:
void Form_lieferanten::slot_new(text_zeilenweise data)
{
    text_zeilenweise param, values;

    param.zeile_anhaengen(PARAM_LIEFERANT_NAME);
    param.zeile_anhaengen(PARAM_LIEFERANT_ERSTELLER);
    param.zeile_anhaengen(PARAM_LIEFERANT_DATERST);
    param.zeile_anhaengen(PARAM_LIEFERANT_KOMMENT);

    values.zeile_anhaengen(data.zeile(1));//Name
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d());
    values.zeile_anhaengen(data.zeile(2));//Kommentar

    dbeigen->data_new(TABNAME_LIEFERANT, param, values);
    update_table();
}

void Form_lieferanten::slot_delete(text_zeilenweise ids)
{
    dbeigen->data_del(TABNAME_LIEFERANT, ids);
    update_table();
}

void Form_lieferanten::slot_edit_dialog(text_zeilenweise ids)
{
    if(ids.zeilenanzahl() == 1)
    {
        idbuffer = ids.zeile(1);
        QString blockfromuser_id = dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, idbuffer);
        QString blockfromuser = dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, idbuffer,\
                                                          TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
        blockfromuser += " ";
        blockfromuser += dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, idbuffer,\
                                                   TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

       if(blockfromuser_id == USER_NOBODY_ID || blockfromuser.isEmpty() )
        {
            text_zeilenweise lieferant;
            QString querryfilter;
            querryfilter += PARAM_LIEFERANT_ID;
            querryfilter += " LIKE \'";
            querryfilter += idbuffer;
            querryfilter += "\'";
            lieferant.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_LIEFERANT, 1, querryfilter).get_text());//Name
            lieferant.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_LIEFERANT, 7, querryfilter).get_text());//Kommenter

            Dialog_lieferanten *d = new Dialog_lieferanten;
            d->setWindowTitle("Lieferant bearbeiten");
            d->set_data(lieferant, idbuffer);
            connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
                    this, SLOT(slot_edit(text_zeilenweise, QString))          );
            connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_dialog_cancel()));

            dbeigen->data_edit(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, user, idbuffer);
            d->exec();
            delete d;
        }else
        {
            Dialog_yes_no *d = new Dialog_yes_no;
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
        mb.setText("Bitte nur einen Lieferanten zum Bearbeiten auswaelen!");
        mb.exec();
    }
}

void Form_lieferanten::slot_edit_dialog()
{
    text_zeilenweise lieferant;
    QString querryfilter;
    querryfilter += PARAM_LIEFERANT_ID;
    querryfilter += " LIKE \'";
    querryfilter += idbuffer;
    querryfilter += "\'";
    lieferant.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_LIEFERANT, 1, querryfilter).get_text());//Name
    lieferant.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_LIEFERANT, 8, querryfilter).get_text());//Kommenter

    Dialog_lieferanten *d = new Dialog_lieferanten;
    d->setWindowTitle("Lieferant bearbeiten");
    d->set_data(lieferant, idbuffer);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
            this, SLOT(slot_edit(text_zeilenweise, QString))          );
    connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_dialog_cancel()));

    dbeigen->data_edit(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, user, idbuffer);
    d->exec();
    delete d;
}

void Form_lieferanten::slot_edit_dialog_cancel()
{
    dbeigen->data_edit(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, USER_NOBODY_ID, idbuffer);
}

void Form_lieferanten::slot_edit(text_zeilenweise data, QString id)
{
    //data:
    //  Wert 1 = Name

    QString blockfromuser_id = dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, idbuffer);
    QString blockfromuser = dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    blockfromuser += " ";
    blockfromuser += dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BLOCK, idbuffer,\
                                               TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

    QString lasteditinguser = dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BEARBEITER, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    lasteditinguser += " ";
    lasteditinguser += dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_BEARBEITER, idbuffer,\
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
        text_zeilenweise param, values;
        datum today;

        param.zeile_anhaengen(PARAM_LIEFERANT_NAME);
        param.zeile_anhaengen(PARAM_LIEFERANT_BEARBEITER);
        param.zeile_anhaengen(PARAM_LIEFERANT_DATBEARB);
        param.zeile_anhaengen(PARAM_LIEFERANT_BLOCK);
        param.zeile_anhaengen(PARAM_LIEFERANT_KOMMENT);

        values.zeile_anhaengen(data.zeile(1));//Name
        values.zeile_anhaengen(user);
        values.zeile_anhaengen(today.get_today_y_m_d());
        values.zeile_anhaengen(USER_NOBODY_ID);
        values.zeile_anhaengen(data.zeile(2));//Kommenter

        dbeigen->data_edit(TABNAME_LIEFERANT, param, values, id);
    }
    update_table();
}

//------------------------------------

