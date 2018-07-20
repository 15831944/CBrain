#include "form_backup.h"
#include "ui_form_backup.h"

Form_backup::Form_backup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_backup)
{
    ui->setupUi(this);
}

Form_backup::~Form_backup()
{
    delete ui;
}

void Form_backup::resizeEvent(QResizeEvent *event)
{
    //int hoehe = this->geometry().height();
    int breite = this->geometry().width();

    ui->pushButton_backup->move(1,1);
    ui->pushButton_restore->move(1, 3 + ui->pushButton_backup->height());

    ui->lineEdit_backupto->move(2+ ui->pushButton_backup->width(),1);
    ui->lineEdit_restorefrom->move(2+ ui->pushButton_backup->width() , 3 + ui->pushButton_backup->height() );

    ui->lineEdit_backupto->setFixedHeight(ui->pushButton_backup->height());
    ui->lineEdit_restorefrom->setFixedHeight(ui->pushButton_backup->height());

    int tmpb = breite - (1+ui->pushButton_backup->width()-1-1);
    if(tmpb < 1)
    {
        tmpb = 1;
    }
    ui->lineEdit_backupto->setFixedWidth(tmpb);
    ui->lineEdit_restorefrom->setFixedWidth(tmpb);


    QWidget::resizeEvent(event);
}

void Form_backup::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_backup::set_ini(inifile *in)
{
    ini = in;
}

void Form_backup::show()
{
    ui->lineEdit_backupto->setText(ini->get_modul_backup_to());
    ui->lineEdit_restorefrom->setText(ini->get_modul_backup_from());
    setVisible(true);
}

void Form_backup::on_pushButton_backup_clicked()
{
    QString msg;
    QString ezpar = "\t";

    text_zeilenweise tables;
    tables = dbeigen->get_tables_tz();

    for(uint i=1; i<= tables.zeilenanzahl() ;i++)
    {
        text_zeilenweise param = dbeigen->get_param_tz(tables.zeile(i));
        text_zeilenweise param_type = dbeigen->get_param_type_tz(tables.zeile(i));
        text_zeilenweise param_primkey = dbeigen->get_param_primkey_tz(tables.zeile(i));
        text_zeilenweise param_extra = dbeigen->get_param_extra_tz(tables.zeile(i));
        text_zeilenweise param_notnull = dbeigen->get_param_notnull_tz(tables.zeile(i));
        text_zeilenweise param_default = dbeigen->get_param_default_tz(tables.zeile(i));

        msg += "<BEGIN_Tabelle>";
        msg += tables.zeile(i);
        msg += "\n";

        msg += "Field"; //Spaltenname
        msg += ezpar;
        msg += "Type";
        msg += ezpar;
        msg += "Null";  //Not Null
        msg += ezpar;
        msg += "Key";   //Is Primary Key
        msg += ezpar;
        msg += "Default"; //Default value
        msg += ezpar;
        msg += "Extra";
        msg += "\n";

        for(uint ii=1; ii<=param.zeilenanzahl() ;ii++)
        {
            msg += param.zeile(ii);
            msg += ezpar;
            msg += param_type.zeile(ii);
            msg += ezpar;
            msg += param_notnull.zeile(ii);
            msg += ezpar;
            msg += param_primkey.zeile(ii);
            msg += ezpar;
            msg += param_default.zeile(ii);
            msg += ezpar;;
            msg += param_extra.zeile(ii);
            msg += ezpar;
            msg += "\n";
        }

        msg += "<ENDE_Tabelle>";
        msg += tables.zeile(i);
        msg += "\n";

    }

    //msg in Datei speichern:

    QString filename;
    filename =  ui->lineEdit_backupto->text();
    filename += QDir::separator();
    filename += "backup_teblestrukture.csv";
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(msg.toUtf8());
        file.close();
    }else
    {
        QMessageBox mb;
        mb.setText("Backup fehlgeschlagen!\nDatei konnte nicht geschrieben werden.");
        mb.exec();
    }
}

void Form_backup::on_pushButton_restore_clicked()
{
    QString filename;
    filename =  ui->lineEdit_restorefrom->text();
    QFile file(filename);

    text_zeilenweise filetext;

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        filetext.set_text(file.readAll());
        //Tabellenstruktur aus der db auslesen:
        text_zeilenweise tables_db = dbeigen->get_tables_tz();

        //Datei auswerten:
        int current_table = -1;
        for(uint i=1; i<=filetext.zeilenanzahl() ;i++)
        {
            QString zeile = filetext.zeile(i);
            if(zeile.contains("<BEGIN_Tabelle>"))
            {
                QString tablename_csv = text_rechts(zeile, "<BEGIN_Tabelle>");
                for(uint ii=1; ii<=tables_db.zeilenanzahl() ;ii++)
                {
                    if(tablename_csv == tables_db.zeile(ii))
                    {
                        current_table = ii;
                        break;
                    }
                }
                if(current_table != -1)//Wenn es die Tabelle bereits gibt
                {
                    //Tabelle ggf ergänzen:
                    //...
                    //...
                    //...
                    //...
                    //...
                }else   //Es gibt die Tabelle noch nicht
                {
                    //Param erfassen:
                    text_zeilenweise param;
                    text_zeilenweise param_type;
                    text_zeilenweise param_notnull;
                    text_zeilenweise param_primkey;
                    text_zeilenweise param_default;
                    text_zeilenweise param_extra;

                    i = i+2;
                    zeile = filetext.zeile(i);
                    for( ; i<=filetext.zeilenanzahl() ;i++)
                    {
                        zeile = filetext.zeile(i);
                        if(zeile.contains("<ENDE_Tabelle>"))
                        {
                            break;
                        }
                        text_zeilenweise zeile_tz;
                        zeile_tz.set_trennzeichen('\t');
                        zeile_tz.set_text(zeile);

                        param.zeile_anhaengen(zeile_tz.zeile(1));
                        param_type.zeile_anhaengen(zeile_tz.zeile(2));
                        param_notnull.zeile_anhaengen(zeile_tz.zeile(3));
                        param_primkey.zeile_anhaengen(zeile_tz.zeile(4));
                        param_default.zeile_anhaengen(zeile_tz.zeile(5));
                        param_extra.zeile_anhaengen(zeile_tz.zeile(6));
                    }

                    //Tabelle anlegen
                    //-------------------------------------
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
                        cmd += tablename_csv;
                        cmd += "(";
                        // not null
                        for(uint ii=1; ii<=param.zeilenanzahl() ;ii++)
                        {
                            QString tmp;
                            tmp = param.zeile(ii);
                            cmd += param.zeile(ii);

                            tmp = param_type.zeile(ii);
                            if(tmp != "---")
                            {
                                cmd += " ";
                                cmd += param_type.zeile(ii);
                            }
                            tmp = param_notnull.zeile(ii);
                            if(tmp == "NO")
                            {
                                cmd += " ";
                                cmd += "NOT NULL";
                            }
                            tmp = param_extra.zeile(ii);
                            if(tmp != "---")
                            {
                                cmd += " ";
                                cmd += param_extra.zeile(ii);
                            }
                            tmp = param_primkey.zeile(ii);
                            if(tmp != "---")
                            {
                                cmd += " ";
                                cmd += "primary key";
                            }
                            tmp = param_default.zeile(ii);
                            if(tmp != "---")
                            {
                                cmd += " DEFAULT \'";
                                cmd += param_default.zeile(ii);
                                cmd += "\'";
                            }

                            if(ii != param.zeilenanzahl())
                            {
                                cmd += ", ";
                            }
                        }
                        cmd += ") ENGINE=InnoDB";

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
                    //-------------------------------------
                }
            }else if(zeile.contains("<ENDE_Tabelle>"))
            {
                current_table = -1;
            }
        }
    }else
    {
        QString msg;
        msg += "Fehler beim Zugriff auf die Datei \"";
        msg += filename;
        msg += "\"!";

        QMessageBox mb;
        mb.setText(msg);
        mb.exec();
    }
}

void Form_backup::on_lineEdit_backupto_editingFinished()
{
    ini->set_modul_backup_to(ui->lineEdit_backupto->text());
    emit signal_save_ini();
}

void Form_backup::on_lineEdit_restorefrom_editingFinished()
{
    ini->set_modul_backup_from(ui->lineEdit_restorefrom->text());
    emit signal_save_ini();
}


