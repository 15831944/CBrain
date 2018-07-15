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
    /*
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
            mb.setText("Fehler bei Datenbankverbindung!");
            mb.exec();
        }
    }
    //-------------------------------------------
    */
}

void Form_lager::on_lineEdit_suche_textChanged()
{
    update_table();
}

//------------------------------------Buttons:

//------------------------------------slots:

//------------------------------------







