#include "form_bestellung.h"
#include "ui_form_bestellung.h"

Form_bestellung::Form_bestellung(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_bestellung)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_bestellung::~Form_bestellung()
{
    delete ui;
    delete model;
}

void Form_bestellung::resizeEvent(QResizeEvent *event)
{
    int b, h;
    b = width();
    h = height();

    int h_btn = ui->lineEdit_suche->height();

    //Suchleiste:
    ui->lineEdit_suche->setFixedWidth(b/3);
    ui->lineEdit_suche->move(b - 1 - ui->lineEdit_suche->width()\
                             ,1);
    ui->label_suche->setFixedHeight(ui->lineEdit_suche->height());
    ui->label_suche->move(b - 1 -ui->lineEdit_suche->width() - 1\
                          - ui->label_suche->width()\
                          ,1);

    //Tabelle:
    ui->tableView->move(1,\
                        1 + h_btn + 1);
    ui->tableView->setFixedWidth(b -2);
    ui->tableView->setFixedHeight(h - ui->tableView->pos().ry() -1);

    QWidget::resizeEvent(event);
}

void Form_bestellung::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_bestellung::set_user(QString u)
{
    user = u;
}

void Form_bestellung::show()
{
    update_table();
    setVisible(true);
}

void Form_bestellung::update_table()
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
            cmd += PARAM_ARTIKEL_PREIS;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LAGERSTAND;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_LAGERST_MIN;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_VE;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTVOR;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTELLT;
            cmd += ", ";
            //------------------------
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_KOMENT;
            //cmd += ", ";
            //------------------------

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
            //------------------------
            cmd += " WHERE ";
            cmd += TABNAME_ARTIKEL;
            cmd += ".";
            cmd += PARAM_ARTIKEL_BESTVOR;
            cmd += " > 0";

            if(!ui->lineEdit_suche->text().isEmpty())
            {
                cmd += " AND";
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

//------------------------------------Buttons:

//------------------------------------private slots:
void Form_bestellung::on_lineEdit_suche_textChanged(const QString &arg1)
{
    update_table();
}

//------------------------------------public slots:

//------------------------------------


