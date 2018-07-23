#include "form_personal.h"
#include "ui_form_personal.h"

Form_personal::Form_personal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_personal)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_personal::~Form_personal()
{
    delete ui;
    delete model;
}

void Form_personal::resizeEvent(QResizeEvent *event)
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

void Form_personal::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_personal::set_user(QString u)
{
    user = u;
}

void Form_personal::show()
{
    update_table();
    setVisible(true);
}

void Form_personal::update_table()
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
            cmd += PARAM_PERSONAL_ID;
            cmd += ", ";
            cmd += PARAM_PERSONAL_VORNAME;
            cmd += ", ";
            cmd += PARAM_PERSONAL_NACHNAME;
            cmd += " FROM ";
            cmd += TABNAME_PERSONAL;
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_PERSONAL_VORNAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_PERSONAL_NACHNAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += PARAM_PERSONAL_NACHNAME;

            if(q.exec(cmd))
            {
                model->setQuery(q);
                ui->tableView->setModel(model);

                QString msg;
                msg += int_to_qstring(model->rowCount());
                msg += " Personen:";
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

void Form_personal::on_lineEdit_suche_textChanged()
{
    update_table();
}

//------------------------------------Buttons:
void Form_personal::on_pushButton_new_clicked()
{
    Dialog_personal *d = new Dialog_personal;
    d->setWindowTitle("Personaleintrag anlegen");
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_new(text_zeilenweise))          );
    d->exec();
    delete d;
}

void Form_personal::on_pushButton_del_clicked()
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
            cmd += TABNAME_PERSONAL;
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_PERSONAL_VORNAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_PERSONAL_NACHNAME;
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
                    tmp += q.value(1).toString();   //Vorname
                    tmp += " ||| ";
                    tmp += q.value(2).toString();   //Nachname
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
    d->setWindowTitle("Personaleintrag loeschen");
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
            this, SLOT(slot_delete(text_zeilenweise))           );
    d->exec();
    delete d;
}

void Form_personal::on_pushButton_edit_clicked()
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
            cmd += TABNAME_PERSONAL;
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += PARAM_PERSONAL_VORNAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += PARAM_PERSONAL_NACHNAME;
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
                    tmp += q.value(1).toString();   //Vorname
                    tmp += " ||| ";
                    tmp += q.value(2).toString();   //Nachname
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
        d->setWindowTitle("Personaleintrag bearbeiten (nur einen)");
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_edit_dialog(text_zeilenweise))      );
        d->exec();
        delete d;
    }
}

//------------------------------------slots:
void Form_personal::slot_new(text_zeilenweise data)
{
    text_zeilenweise param, values;

    param.zeile_anhaengen(PARAM_PERSONAL_VORNAME);
    param.zeile_anhaengen(PARAM_PERSONAL_NACHNAME);


    values.zeile_anhaengen(data.zeile(1));//Vorname
    values.zeile_anhaengen(data.zeile(2));//Nachname

    dbeigen->data_new(TABNAME_PERSONAL, param, values);
    update_table();
}

void Form_personal::slot_delete(text_zeilenweise ids)
{
    dbeigen->data_del(TABNAME_PERSONAL, ids);
    update_table();
}

void Form_personal::slot_edit_dialog(text_zeilenweise ids)
{
    if(ids.zeilenanzahl() == 1)
    {
        idbuffer = ids.zeile(1);
        text_zeilenweise person;

        QString querryfilter;
        querryfilter += PARAM_PERSONAL_ID;
        querryfilter += " LIKE \'";
        querryfilter += idbuffer;
        querryfilter += "\'";
        person.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_PERSONAL, 1, querryfilter).get_text());//Vorame
        person.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_PERSONAL, 2, querryfilter).get_text());//Nachname

        Dialog_personal *d = new Dialog_personal;
        d->setWindowTitle("Personaleintrag bearbeiten");
        d->set_data(person, idbuffer);
        connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
                this, SLOT(slot_edit(text_zeilenweise, QString))          );

        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte nur einnen Eintrag zum Bearbeiten auswaelen!");
        mb.exec();
    }
}

void Form_personal::slot_edit(text_zeilenweise data, QString id)
{
    text_zeilenweise param, values;

    param.zeile_anhaengen(PARAM_PERSONAL_VORNAME);
    param.zeile_anhaengen(PARAM_PERSONAL_NACHNAME);

    values.zeile_anhaengen(data.zeile(1));
    values.zeile_anhaengen(data.zeile(2));

    dbeigen->data_edit(TABNAME_PERSONAL, param, values, id);
    update_table();
}


