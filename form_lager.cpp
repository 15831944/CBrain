#include "form_lager.h"
#include "ui_form_lager.h"

Form_lager::Form_lager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_lager)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
    this->model_artikel = new QSqlQueryModel();
}

Form_lager::~Form_lager()
{
    delete ui;
    delete model;
    delete model_artikel;
}

void Form_lager::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->geometry().height();
    int breite = this->geometry().width();

    //Buttons:
    ui->pushButton_in->move(1,1);
    ui->pushButton_in->setFixedWidth(breite/5);

    ui->pushButton_out->move(1 + ui->pushButton_in->geometry().width() + 1\
                             ,1);
    ui->pushButton_out->setFixedWidth(ui->pushButton_in->geometry().width());

    //Suchleiste:
    ui->lineEdit_suche->setFixedHeight(ui->pushButton_in->geometry().height());
    ui->lineEdit_suche->setFixedWidth(breite/3);
    ui->lineEdit_suche->move(breite - 1 - ui->lineEdit_suche->geometry().width()\
                             ,1);

    ui->label_suche->setFixedHeight(ui->pushButton_in->geometry().height());
    ui->label_suche->move(breite - 1 -ui->lineEdit_suche->geometry().width() - 1\
                          - ui->label_suche->geometry().width()\
                          ,1);

    //Tabelle Lager:
    ui->tableView->move(1,\
                        1 + ui->pushButton_in->geometry().height() + 1);
    ui->tableView->setFixedWidth( (breite-2)/3*2 );
    ui->tableView->setFixedHeight(hoehe - ui->tableView->pos().ry() -1);

    //Tabelle Artikel:
    ui->tableView_artikel->move(1 + ui->tableView->geometry().width() + 1,\
                                1 + ui->pushButton_in->geometry().height() + 1);
    ui->tableView_artikel->setFixedWidth( (breite-2)/3 );
    ui->tableView_artikel->setFixedHeight(ui->tableView->geometry().height());

    QWidget::resizeEvent(event);
}

void Form_lager::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_lager::set_user(QString u)
{
    user = u;
}

void Form_lager::show()
{
    update_table();
    setVisible(true);
}

void Form_lager::update_table()
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
            //-------------------------------------------
            cmd += "SELECT ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_VORGANG;
            cmd += " AS ";
            cmd += "Vorgang";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += " AS ";
            cmd += "Artikelbez";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_NR;
            cmd += " AS ";
            cmd += "Artikelnr";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_MENGE;
            cmd += " AS ";
            cmd += "Menge";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_NAME;
            cmd += " AS ";
            cmd += "Kommission";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_KOMMENT;
            cmd += " AS ";
            cmd += "Kommentar";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_ERSTELLER;
            cmd += " AS ";
            cmd += "Ersteller";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_DATERST;
            cmd += " AS ";
            cmd += "Erfassung";
            //cmd += ", ";
            //------------------------
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_LAGER;
            //------------------------
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_ARTIKEL;
            cmd += " ON ";
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_ARTIKELID;
            cmd += " = ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_ID;
            //------------------------
            //------------------------
            cmd += " LEFT JOIN ";
            cmd += TABNAME_PROJEKT;
            cmd += " ON ";
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_KOMMISSION;
            cmd += " = ";
            cmd += TABNAME_PROJEKT;
            cmd += ".";
            cmd += PARAM_PROJEKT_ID;
            //------------------------
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_PROJEKT;
                cmd += ".";
                cmd += PARAM_PROJEKT_NAME;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_DATERST;
            cmd += " DESC";
            //------------------------

            if(q.exec(cmd))
            {
                model->setQuery(q);
                ui->tableView->setModel(model);

                QString msg;
                msg += int_to_qstring(model->rowCount());
                msg += " Buchungen:";
                ui->label_suche->setText(msg);

            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            //-------------------------------------------
            //-------------------------------------------
            cmd.clear();
            cmd += "SELECT ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_NR;
            cmd += " AS ";
            cmd += "Artikelnummer";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += " AS ";
            cmd += "Artikelbez";
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LAGERSTAND;
            cmd += " AS ";
            cmd += "Lagerstand";
            //cmd += ", ";
            //------------------------
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;
            //------------------------
            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " WHERE ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_NR;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
                cmd += " OR ";
                cmd += TABNAME_ARTIKEL;
                cmd += ".";
                cmd += PARAM_ARTIKEL_BEZ;
                cmd += " LIKE \'%";
                cmd += ui->lineEdit_suche->text();
                cmd += "%\'";
            }
            //------------------------
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_NR;
            //------------------------

            if(q.exec(cmd))
            {
                model_artikel->setQuery(q);
                ui->tableView_artikel->setModel(model_artikel);
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            //-------------------------------------------
            //-------------------------------------------
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

void Form_lager::on_lineEdit_suche_textChanged()
{
    update_table();
}

//------------------------------------Buttons:
void Form_lager::on_pushButton_in_clicked()
{
    Dialog_lager *d = new Dialog_lager;
    d->set_db(dbeigen);
    d->setup();
    d->setWindowTitle("Waren-Eingang buchen");
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_in(text_zeilenweise))          );
    d->exec();
    delete d;


}

void Form_lager::on_pushButton_out_clicked()
{
    Dialog_lager *d = new Dialog_lager;
    d->set_db(dbeigen);
    d->setup();
    d->setWindowTitle("Waren-Ausgang buchen");
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),  \
            this, SLOT(slot_out(text_zeilenweise))          );
    d->exec();
    delete d;
}

//------------------------------------slots:
void Form_lager::slot_in(text_zeilenweise data)
{
    text_zeilenweise param, values;

    QString artikelid   = data.zeile(1);
    QString menge       = data.zeile(2);
    QString projektid   = data.zeile(3);
    QString kommentar   = data.zeile(4);

    param.zeile_anhaengen(PARAM_LAGER_VORGANG);
    param.zeile_anhaengen(PARAM_LAGER_ARTIKELID);
    param.zeile_anhaengen(PARAM_LAGER_MENGE);
    param.zeile_anhaengen(PARAM_LAGER_ERSTELLER);
    param.zeile_anhaengen(PARAM_LAGER_DATERST);
    param.zeile_anhaengen(PARAM_LAGER_KOMMISSION);
    param.zeile_anhaengen(PARAM_LAGER_KOMMENT);

    values.zeile_anhaengen(VORGANG_WARENEINGANG);
    values.zeile_anhaengen(artikelid);
    values.zeile_anhaengen(menge);
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d_h_m_s());
    values.zeile_anhaengen(projektid);
    values.zeile_anhaengen(kommentar);

    //Aktuelle Lagermenge des Artikeln abfragen:
    QString menge_vorher = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, artikelid);
    int menge_akt = menge_vorher.toInt() + menge.toInt();

    dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, int_to_qstring(menge_akt), artikelid);

    dbeigen->data_new(TABNAME_LAGER, param, values);
    update_table();
}

void Form_lager::slot_out(text_zeilenweise data)
{
    text_zeilenweise param, values;

    QString artikelid   = data.zeile(1);
    QString menge       = data.zeile(2);
    QString projektid   = data.zeile(3);
    QString kommentar   = data.zeile(4);

    param.zeile_anhaengen(PARAM_LAGER_VORGANG);
    param.zeile_anhaengen(PARAM_LAGER_ARTIKELID);
    param.zeile_anhaengen(PARAM_LAGER_MENGE);
    param.zeile_anhaengen(PARAM_LAGER_ERSTELLER);
    param.zeile_anhaengen(PARAM_LAGER_DATERST);
    param.zeile_anhaengen(PARAM_LAGER_KOMMISSION);
    param.zeile_anhaengen(PARAM_LAGER_KOMMENT);

    values.zeile_anhaengen(VORGANG_WARENEINGANG);
    values.zeile_anhaengen(artikelid);
    values.zeile_anhaengen(menge);
    values.zeile_anhaengen(user);
    datum heute;
    values.zeile_anhaengen(heute.get_today_y_m_d());
    values.zeile_anhaengen(projektid);
    values.zeile_anhaengen(kommentar);

    //Aktuelle Lagermenge des Artikeln abfragen:
    QString menge_vorher = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, artikelid);
    int menge_akt = menge_vorher.toInt() - menge.toInt();

    if(menge_akt >= 0)
    {
        dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_LAGERSTAND, int_to_qstring(menge_akt), artikelid);

        dbeigen->data_new(TABNAME_LAGER, param, values);
        update_table();
    }else
    {
        QString msg;
        msg += "Der Artikel ist derzeit noch ";
        msg += menge_vorher;
        msg += " x im Lager.\n";
        msg += "Sie koennen nicht ";
        msg += menge;
        msg += " x entnehmen!";

        QMessageBox mb;
        mb.setText(msg);
        mb.exec();
    }
}

//------------------------------------











