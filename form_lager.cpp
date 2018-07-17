#include "form_lager.h"
#include "ui_form_lager.h"

Form_lager::Form_lager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_lager)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_lager::~Form_lager()
{
    delete ui;
    delete model;
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

    //Tabelle:
    ui->tableView->move(1,\
                        1 + ui->pushButton_in->geometry().height() + 1);
    ui->tableView->setFixedWidth(breite -2);
    ui->tableView->setFixedHeight(hoehe - ui->tableView->pos().ry() -1);

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
            cmd += " ORDER BY ";            //Sortiert nach:
            cmd += TABNAME_LAGER;
            cmd += ".";
            cmd += PARAM_LAGER_DATERST;

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

//------------------------------------slots:

//------------------------------------







