#include "form_matlist.h"
#include "ui_form_matlist.h"

Form_matlist::Form_matlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_matlist)
{
    ui->setupUi(this);
    ui->lineEdit_projekt->setText("Projekt?");
}

Form_matlist::~Form_matlist()
{
    delete ui;
}

void Form_matlist::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_matlist::set_user(QString u)
{
    user = u;
}

void Form_matlist::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->height();
    int breite = this->width();

    int b_li = breite/5*3;
    int b_re = breite - b_li - 4;   //Rand_li = 1 + Rand_re = 1 + Rand_mi = 2 == 4

    int h_btn = ui->pushButton_projektauswahl->height();

    //links Kopfzeile:
    ui->lineEdit_projekt_id->setFixedHeight(h_btn);
    ui->lineEdit_projekt_id->move(1,1);

    ui->lineEdit_projekt->setFixedHeight(h_btn);
    ui->lineEdit_projekt->move(1 + ui->lineEdit_projekt_id->width() + 1\
                               ,1);

    ui->pushButton_projektauswahl->move(1 + ui->lineEdit_projekt_id->width() + \
                                        1 + ui->lineEdit_projekt->width() + 1,\
                                        1);

    ui->lineEdit_filter->setFixedHeight(h_btn);
    ui->lineEdit_filter->setFixedWidth(b_li/3);
    ui->lineEdit_filter->move(b_li - ui->lineEdit_filter->width(),\
                              1);
    ui->label_filter->setFixedHeight(h_btn);
    ui->label_filter->move(b_li - ui->lineEdit_filter->width() - 1 - ui->label_filter->width(),\
                           1);
    int restbr_li = b_li - 4 - ui->lineEdit_projekt->width() - ui->lineEdit_projekt_id->width() -\
                    ui->pushButton_projektauswahl->width() - ui->lineEdit_filter->width();
    if(restbr_li < ui->label_filter->width())
    {
        ui->label_filter->hide();
    }else
    {
        ui->label_filter->show();
    }

    //links Tabelle:
    ui->tableView->setFixedSize(b_li,\
                                hoehe - 1 - h_btn - 1 - 1);
    ui->tableView->move(1,\
                        1 + h_btn + 1);

    //rechts Kopfzeile:
    ui->pushButton_check_all_pos->setFixedWidth(b_re/2-2);
    ui->pushButton_check_all_pos->move(1 + b_li + 2,\
                                       1);

    ui->pushButton_check_activ_pos->setFixedWidth(ui->pushButton_check_all_pos->width());
    ui->pushButton_check_activ_pos->move(1 + b_li + 2 + ui->pushButton_check_all_pos->width() + 1,\
                                      1);

    //rechts Listwidget + Buttons
    ui->listWidget_matpos->setFixedSize(b_re,\
                                        hoehe - 1*5 - h_btn*3);
    ui->listWidget_matpos->move(1 + b_li + 2,\
                                1 + h_btn + 1);

    ui->pushButton_pos_new->setFixedWidth(b_re/3 - 2*2);
    ui->pushButton_pos_edit->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_delete->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_copy->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_import->setFixedWidth(ui->pushButton_pos_new->width());

    ui->pushButton_pos_new->move(1 + b_li + 2,\
                                 1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_edit->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                  1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_delete->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                    1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_copy->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                  1 + h_btn + 1 + ui->listWidget_matpos->height() + 1 + h_btn + 1);
    ui->pushButton_pos_import->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                    1 + h_btn + 1 + ui->listWidget_matpos->height() + 1 + h_btn + 1);

    QWidget::resizeEvent(event);
}

void Form_matlist::on_pushButton_projektauswahl_clicked()
{
    text_zeilenweise projekte, ids;
    projekte = dbeigen->get_data_tz(TABNAME_PROJEKT, PARAM_PROJEKT_NAME);
    ids = dbeigen->get_data_tz(TABNAME_PROJEKT, PARAM_PROJEKT_ID);

    Dialog_dataselection *d = new Dialog_dataselection;
    d->set_data(projekte, ids);
    d->setWindowTitle("Projektauswahl");
    d->set_anz_returnwerte(1);
    connect(d, SIGNAL(signal_send_selection(text_zeilenweise)), \
            this, SLOT(slot_set_project(text_zeilenweise))      );
    d->exec();

    delete d;
}

void Form_matlist::slot_set_project(text_zeilenweise p)
{
    ui->lineEdit_projekt_id->setText(p.zeile(1));
    ui->lineEdit_projekt->setText(dbeigen->get_data_qstring(TABNAME_PROJEKT, PARAM_PROJEKT_NAME, p.zeile(1)));
}

void Form_matlist::on_lineEdit_projekt_id_textChanged(const QString &arg1)
{
    if(!arg1.isEmpty())
    {
        //Die Tabelle "promatpos_***" anlegen wenn es sie noch nicht gibt
        // *** == Wert von arg1 == ID des Projektes
        create_table_promatpos();
        update_listwidget_matpos();
    }
}

void Form_matlist::create_table_promatpos()
{
    if(!ui->lineEdit_projekt_id->text().isEmpty())
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
            cmd += TABNAME_PROMATPOS;
            cmd += ui->lineEdit_projekt_id->text();
            cmd += "(";
            cmd += "id int(11) unsigned auto_increment primary key not null";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_BEZ;
            cmd += " varchar(100)";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_MENGE;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_ERSTELLER;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_BEARBEITER;
            cmd += " int(11) unsigned";
            cmd += ", ";
            cmd += PARAM_PROMATPOS_BLOCK;
            cmd += " int(11) unsigned";
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

void Form_matlist::update_listwidget_matpos()
{
    //muss noch programmiert werden
}

















