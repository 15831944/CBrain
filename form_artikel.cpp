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

void Form_artikel::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_artikel::set_user(QString u)
{
    user = u;
}

void Form_artikel::show()
{
    update_table();
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
            cmd += PARAM_ARTIKEL_NR;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_LIEFERANT;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_LAGERSTAND;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_ERSTELLER;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_DATERST;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_BEARBEITER;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_DATBEARB;
            cmd += " FROM ";
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
                model->setQuery(q);
                ui->tableView->setModel(model);
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

void Form_artikel::on_lineEdit_suche_textChanged()
{
    update_table();
}

//------------------------------------Buttons:
void Form_artikel::on_pushButton_new_clicked()
{
    Dialog_artikel *d = new Dialog_artikel;
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
            mb.setText("Fehler bei Datenbankverbindung!");
            mb.exec();
        }
    }
    //-------------------------------------------
    Dialog_dataselection *d = new Dialog_dataselection;
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
        d->setWindowTitle("Artikel bearbeiten (nur einen)");
        connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
                this, SLOT(slot_edit_dialog(text_zeilenweise))      );
        d->exec();
        delete d;
    }
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

    values.zeile_anhaengen(data.zeile(1));
    values.zeile_anhaengen(data.zeile(2));
    values.zeile_anhaengen(data.zeile(3));
    values.zeile_anhaengen("0");
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d());

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
        text_zeilenweise artikel;
                        //Wert 1 = Artikelnummer
                        //Wert 2 = Bezeichnung
                        //Wert 3 = Lieferant
        QString querryfilter;
        querryfilter += PARAM_ARTIKEL_ID;
        querryfilter += " LIKE \'";
        querryfilter += ids.zeile(1);
        querryfilter += "\'";
        artikel.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_ARTIKEL, 1, querryfilter).get_text());//Nr
        artikel.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_ARTIKEL, 2, querryfilter).get_text());//Bez
        artikel.zeile_anhaengen(dbeigen->get_values_from_column(TABNAME_ARTIKEL, 3, querryfilter).get_text());//Lieferant


        Dialog_artikel *d = new Dialog_artikel;
        d->set_db(dbeigen);
        d->setup();
        d->set_data(artikel, ids.zeile(1));
        connect(d, SIGNAL(signal_send_data(text_zeilenweise, QString)),  \
                this, SLOT(slot_edit(text_zeilenweise, QString))          );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte nur einen Artikel zum Bearbeiten auswaelen!");
        mb.exec();
    }

}

void Form_artikel::slot_edit(text_zeilenweise data, QString id)
{
    //data:
    //  Wert 1 = Artikelnummer
    //  Wert 2 = Bezeichnung
    //  Wert 3 = Lieferant
    bool ok = true;

    ok = dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_NR, data.zeile(1), id);
    if(ok == true)
    {
        ok = dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZ, data.zeile(2), id);
    }
    if(ok == true)
    {
        ok = dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_LIEFERANT, data.zeile(3), id);
    }
    if(ok == true)
    {
        ok = dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEARBEITER, user, id);
    }
    if(ok == true)
    {
        datum today;
        ok = dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_DATBEARB, today.get_today_y_m_d(), id);
    }
    update_table();
}

//------------------------------------
