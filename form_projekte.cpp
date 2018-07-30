#include "form_projekte.h"
#include "ui_form_projekte.h"

Form_projekte::Form_projekte(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_projekte)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_projekte::~Form_projekte()
{
    delete ui;
    delete model;
}

void Form_projekte::resizeEvent(QResizeEvent *event)
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

void Form_projekte::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_projekte::set_user(QString u)
{
    user = u;
}

void Form_projekte::show()
{
    update_table();
    setVisible(true);
}

void Form_projekte::update_table()
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
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_NAME;
            cmd += ", ";
            //------------------------
            cmd += PARAM_PROJEKT_KOMMENT;
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
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_DATERST;
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
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_DATBEARB;
            //cmd += ", ";
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_PROJEKT;
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_PERSONAL;
            cmd += " AS ";
            cmd += "E";
            cmd += " ON (";
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_ERSTELLER;
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
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_BEARBEITER;
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
                cmd += PARAM_PROJEKT_ID;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_PROJEKT_NAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_ID;
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_NAME;
            //------------------------

            if(q.exec(cmd))
            {
                model->setQuery(q);
                ui->tableView->setModel(model);

                QString msg;
                msg += int_to_qstring(model->rowCount());
                msg += " Projekte:";
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

void Form_projekte::on_lineEdit_suche_textChanged()
{
    update_table();
}

void Form_projekte::create_table_promat(QString project_id)
{
    if(!project_id.isEmpty())
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
            cmd += TABNAME_PROMAT;
            cmd += project_id;
            cmd += "(";
            cmd += "id int(11) unsigned auto_increment primary key not null";
            cmd += ", ";
            cmd += PARAM_PROMAT_ARTIKEL_ID;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMAT_ME_BESTELLT;
            cmd += " int(11)";
            cmd += ", ";
            cmd += PARAM_PROMAT_ME_GELIEFERT;
            cmd += " int(11)";
            cmd += ", ";
            cmd += PARAM_PROMAT_ME_VERARBEITET;
            cmd += " int(11)";
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
//------------------------------------Buttons:
void Form_projekte::on_pushButton_new_clicked()
{
    Dialog_projekte *d = new Dialog_projekte;
    d->setWindowTitle("Projekt anlegen");
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_new(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_projekte::on_pushButton_del_clicked()
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
            cmd += TABNAME_PROJEKT;
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_PROJEKT_NAME;
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
    d->setWindowTitle("Projekt loeschen");
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
            this, SLOT(slot_delete(text_zeilenweise))           );
    d->exec();
    delete d;
}

void Form_projekte::on_pushButton_edit_clicked()
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
            cmd += TABNAME_PROJEKT;
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_PROJEKT_NAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " GROUP BY ";
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_ID;
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_NAME;
            //------------------------

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
        d->setWindowTitle("Projekt bearbeiten (nur einen)");
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_edit_dialog(text_zeilenweise))      );
        d->exec();
        delete d;
    }
}

//------------------------------------slots:
void Form_projekte::slot_new(text_zeilenweise data)
{
    text_zeilenweise param, values;

    param.zeile_anhaengen(PARAM_PROJEKT_NAME);
    param.zeile_anhaengen(PARAM_PROJEKT_ERSTELLER);
    param.zeile_anhaengen(PARAM_PROJEKT_DATERST);
    param.zeile_anhaengen(PARAM_PROJEKT_KOMMENT);

    values.zeile_anhaengen(data.zeile(1));//Name
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d());
    values.zeile_anhaengen(data.zeile(2));//Kommentar

    dbeigen->data_new(TABNAME_PROJEKT, param, values);
    update_table();
    create_table_promat(dbeigen->get_highest_id(TABNAME_PROJEKT));
}

void Form_projekte::slot_delete(text_zeilenweise ids)
{
    dbeigen->data_del(TABNAME_PROJEKT, ids);
    update_table();
}

void Form_projekte::slot_edit_dialog(text_zeilenweise ids)
{
    if(ids.zeilenanzahl() == 1)
    {
        idbuffer = ids.zeile(1);
        QString blockfromuser_id = dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, idbuffer);
        QString blockfromuser = dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, idbuffer,\
                                                          TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
        blockfromuser += " ";
        blockfromuser += dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, idbuffer,\
                                                   TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

        if(blockfromuser_id == USER_NOBODY_ID || blockfromuser.isEmpty() )
        {
            text_zeilenweise projekt;
                            //Wert 1 = Name
            QString querryfilter;
            querryfilter += PARAM_PROJEKT_ID;
            querryfilter += " LIKE \'";
            querryfilter += idbuffer;
            querryfilter += "\'";
            projekt.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_PROJEKT, 1, querryfilter).get_text());//Name
            projekt.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_PROJEKT, 7, querryfilter).get_text());//Kommentar

            Dialog_projekte *d = new Dialog_projekte;
            d->setWindowTitle("Projekt bearbeiten");
            d->set_data(projekt, idbuffer);
            connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
                    this, SLOT(slot_edit(text_zeilenweise, QString))          );
            connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_dialog_cancel()));

            dbeigen->data_edit(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, user, idbuffer);
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
        mb.setText("Bitte nur ein Projekt zum Bearbeiten auswaelen!");
        mb.exec();
    }
}

void Form_projekte::slot_edit_dialog()
{
    text_zeilenweise projekt;
                    //Wert 1 = Name
    QString querryfilter;
    querryfilter += PARAM_PROJEKT_ID;
    querryfilter += " LIKE \'";
    querryfilter += idbuffer;
    querryfilter += "\'";
    projekt.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_PROJEKT, 1, querryfilter).get_text());//Name
    projekt.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_PROJEKT, 7, querryfilter).get_text());//Kommentar

    Dialog_projekte *d = new Dialog_projekte;
    d->setWindowTitle("Projekt bearbeiten");
    d->set_data(projekt, idbuffer);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
            this, SLOT(slot_edit(text_zeilenweise, QString))          );
    connect(d, SIGNAL(signal_cancel()), this, SLOT(slot_edit_dialog_cancel()));

    dbeigen->data_edit(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, user, idbuffer);
    d->exec();
    delete d;
}

void Form_projekte::slot_edit_dialog_cancel()
{
    dbeigen->data_edit(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, USER_NOBODY_ID, idbuffer);
}

void Form_projekte::slot_edit(text_zeilenweise data, QString id)
{
    //data:
    //  Wert 1 = Name

    QString blockfromuser_id = dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, idbuffer);
    QString blockfromuser = dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    blockfromuser += " ";
    blockfromuser += dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BLOCK, idbuffer,\
                                               TABNAME_PERSONAL, PARAM_PERSONAL_NACHNAME);

    QString lasteditinguser = dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BEARBEITER, idbuffer,\
                                                      TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME);
    lasteditinguser += " ";
    lasteditinguser += dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_BEARBEITER, idbuffer,\
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

        param.zeile_anhaengen(PARAM_PROJEKT_NAME);
        param.zeile_anhaengen(PARAM_PROJEKT_BEARBEITER);
        param.zeile_anhaengen(PARAM_PROJEKT_DATBEARB);
        param.zeile_anhaengen(PARAM_PROJEKT_BLOCK);
        param.zeile_anhaengen(PARAM_PROJEKT_KOMMENT);

        values.zeile_anhaengen(data.zeile(1));
        values.zeile_anhaengen(user);
        values.zeile_anhaengen(today.get_today_y_m_d());
        values.zeile_anhaengen(USER_NOBODY_ID);
        values.zeile_anhaengen(data.zeile(2));

        dbeigen->data_edit(TABNAME_PROJEKT, param, values, id);
    }
    update_table();
}

//------------------------------------

















