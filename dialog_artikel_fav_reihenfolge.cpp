#include "dialog_artikel_fav_reihenfolge.h"
#include "ui_dialog_artikel_fav_reihenfolge.h"

Dialog_artikel_fav_reihenfolge::Dialog_artikel_fav_reihenfolge(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_artikel_fav_reihenfolge)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Dialog_artikel_fav_reihenfolge::~Dialog_artikel_fav_reihenfolge()
{
    delete ui;
}

void Dialog_artikel_fav_reihenfolge::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
    update_favorit();
}

void Dialog_artikel_fav_reihenfolge::update_favorit()
{
    text_zeilenweise id, nr, bez, rang;

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
            cmd += "SELECT ";
            cmd += PARAM_ARTIKEL_ID;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_NR;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_BEZ;
            cmd += ", ";
            cmd += PARAM_ARTIKEL_FAVORDER;
            cmd += " FROM ";
            cmd += TABNAME_ARTIKEL;

            cmd += " WHERE ";
            cmd += PARAM_ARTIKEL_ISFAVORIT;
            cmd += " LIKE 1";

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    id.zeile_anhaengen(q.value(0).toString());
                    nr.zeile_anhaengen(q.value(1).toString());
                    bez.zeile_anhaengen(q.value(2).toString());
                    rang.zeile_anhaengen(q.value(3).toString());
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

        int rang_max = 0;
        for(uint i=1; i<=rang.zeilenanzahl() ;i++)
        {
            int tmp = rang.zeile(i).toInt();
            if(tmp > rang_max)
            {
                rang_max = tmp;
            }
        }
        text_zeilenweise tz;
        //Artikel dem Rang nach sortieren:
        for(int i=1; i<=rang_max ;i++)
        {
            for(uint ii=1; ii<=rang.zeilenanzahl() ;ii++)
            {
                int rang_akt = rang.zeile(ii).toInt();
                if(rang_akt == i)
                {
                    QString tmp;
                    tmp += id.zeile(ii);
                    tmp += " ||| ";
                    tmp += nr.zeile(ii);
                    tmp += " ||| ";
                    tmp += bez.zeile(ii);
                    tz.zeile_anhaengen(tmp);
                }
            }
        }
        //Artikel ohne Rang anhängen:
        for(uint i=1; i<=rang.zeilenanzahl() ;i++)
        {
            int rang_akt = rang.zeile(i).toInt();
            if(rang_akt == 0)
            {
                QString tmp;
                tmp += id.zeile(i);
                tmp += " ||| ";
                tmp += nr.zeile(i);
                tmp += " ||| ";
                tmp += bez.zeile(i);
                tz.zeile_anhaengen(tmp);
            }
        }
        //Artikel in listwidget schreiben:
        for(uint i=1; i<=tz.zeilenanzahl() ;i++)
        {
           ui->listWidget->addItem(tz.zeile(i));
        }
    }
}

void Dialog_artikel_fav_reihenfolge::on_pushButton_ok_clicked()
{
    for(int i=1; i<=ui->listWidget->count();i++)
    {
        QString id;
        id = ui->listWidget->item(i-1)->text();
        id = text_links(id, " ||| ");
        if(!dbeigen->data_edit(TABNAME_ARTIKEL, PARAM_ARTIKEL_FAVORDER, int_to_qstring(i),id))
        {
            break;
        }
    }
    this->close();
}

void Dialog_artikel_fav_reihenfolge::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_artikel_fav_reihenfolge::on_pushButton_up_clicked()
{
    if(ui->listWidget->currentRow() >= 1)
    {
        text_zeilenweise tz;
        for(int i=1; i<=ui->listWidget->count() ;i++)
        {
            tz.zeile_anhaengen(ui->listWidget->item(i-1)->text());
        }
        int akt_zeile = ui->listWidget->currentRow()+1;
        QString akt_zeilentext = ui->listWidget->item(akt_zeile-1)->text();
        tz.zeile_loeschen(akt_zeile);
        tz.zeile_einfuegen(akt_zeile-2 ,akt_zeilentext);
        ui->listWidget->clear();
        for(uint i=1; i<=tz.zeilenanzahl() ;i++)
        {
           ui->listWidget->addItem(tz.zeile(i));
        }
        ui->listWidget->setCurrentRow(akt_zeile-2);
    }
}

void Dialog_artikel_fav_reihenfolge::on_pushButton_down_clicked()
{
    if(ui->listWidget->currentRow() != -1  &&\
       ui->listWidget->currentRow() != ui->listWidget->count()-1 )
    {
        text_zeilenweise tz;
        for(int i=1; i<=ui->listWidget->count() ;i++)
        {
            tz.zeile_anhaengen(ui->listWidget->item(i-1)->text());
        }
        int akt_zeile = ui->listWidget->currentRow()+1;
        QString akt_zeilentext = ui->listWidget->item(akt_zeile-1)->text();
        tz.zeile_loeschen(akt_zeile);
        if(akt_zeile != tz.zeilenanzahl())
        {
            tz.zeile_einfuegen(akt_zeile ,akt_zeilentext);
        }else
        {
            tz.zeile_anhaengen(akt_zeilentext);
        }

        ui->listWidget->clear();
        for(uint i=1; i<=tz.zeilenanzahl() ;i++)
        {
           ui->listWidget->addItem(tz.zeile(i));
        }
        ui->listWidget->setCurrentRow(akt_zeile);
    }
}
