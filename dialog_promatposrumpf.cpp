#include "dialog_promatposrumpf.h"
#include "ui_dialog_promatposrumpf.h"

Dialog_promatposrumpf::Dialog_promatposrumpf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_promatposrumpf)
{
    ui->setupUi(this);
    current_userid = "0";
    this->model = new QSqlQueryModel();
    //setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
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
    ui->tableView->setFixedHeight(hoehe - 3 - h_btn - ui->lineEdit_filter->height());
    ui->tableView->move(1, 1 + h_btn + 1 + ui->lineEdit_filter->height() - 1);

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
        cmd += PARAM_STATUS_STATUS;
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
    text_zeilenweise fav;

    if(dbeigen != NULL)
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
                    QString tmp;
                    tmp += q.value(0).toString();
                    tmp += " ||| ";
                    tmp += q.value(1).toString();
                    tmp += " ||| ";
                    tmp += q.value(2).toString();
                    fav.zeile_anhaengen(tmp);
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
        for(uint i=1; i<=fav.zeilenanzahl() ;i++)
        {
            ui->listWidget_favourite->addItem(fav.zeile(i));
        }
    }
}
