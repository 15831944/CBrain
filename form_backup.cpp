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
    if(dbeigen != NULL)
    {
        int breite = this->width();
        int hbtn = ui->pushButton_backup->height();
        int bbtn = 220;

        ui->pushButton_backup->setFixedWidth(bbtn);
        ui->pushButton_backup_all->setFixedWidth(bbtn);
        ui->pushButton_restore->setFixedWidth(bbtn);
        ui->pushButton_restore_all->setFixedWidth(bbtn);

        ui->pushButton_backup_all->setFixedHeight(hbtn);
        ui->pushButton_restore->setFixedHeight(hbtn);
        ui->pushButton_restore_all->setFixedHeight(hbtn);

        ui->pushButton_backup->move(1,\
                                    1);
        ui->pushButton_restore->move(1, \
                                     1 + hbtn + 1);
        ui->pushButton_backup_all->move(1, \
                                        1 + (hbtn + 1)*2);
        ui->pushButton_restore_all->move(1, \
                                         1 + (hbtn + 1)*3);

        ui->lineEdit_backupto->setFixedHeight(hbtn);
        ui->lineEdit_restorefrom->setFixedHeight(hbtn);

        ui->lineEdit_backupto->move(1 + bbtn + 1,\
                                    1);
        ui->lineEdit_restorefrom->move(1 + bbtn + 1,\
                                       1 + hbtn + 1 );



        int tmpb = breite - (1 + bbtn + 1 + 1);
        if(tmpb < 1)
        {
            tmpb = 1;
        }
        ui->lineEdit_backupto->setFixedWidth(tmpb);
        ui->lineEdit_restorefrom->setFixedWidth(tmpb);

        QWidget::resizeEvent(event);
    }
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
    if(dbeigen != NULL)
    {
        ui->lineEdit_backupto->setText(ini->get_modul_backup_to());
        ui->lineEdit_restorefrom->setText(ini->get_modul_backup_from());
        setVisible(true);
    }else
    {
        QMessageBox mb;
        mb.setText(tr("dbeigen == NULL!"));
        mb.exec();
    }
}

void Form_backup::on_pushButton_backup_clicked()
{
    QString msg;
    QString ezpar = "\t";

    text_zeilenweise tables;
    tables = dbeigen->get_tables_tz();

    for(uint i=1; i<= tables.zeilenanzahl() ;i++)
    {
        if(  !tables.zeile(i).contains(TABNAME_PROMATPOSLIST) && \
             !tables.zeile(i).contains(TABNAME_PROMATPOS) && \
             !tables.zeile(i).contains(TABNAME_PROMAT)  )
        {
            text_zeilenweise param = dbeigen->get_param_tz(tables.zeile(i));
            text_zeilenweise param_type = dbeigen->get_param_type_tz(tables.zeile(i));
            text_zeilenweise param_primkey = dbeigen->get_param_primkey_tz(tables.zeile(i));
            text_zeilenweise param_extra = dbeigen->get_param_extra_tz(tables.zeile(i));
            text_zeilenweise param_notnull = dbeigen->get_param_notnull_tz(tables.zeile(i));
            text_zeilenweise param_default = dbeigen->get_param_default_tz(tables.zeile(i));

            msg += "<BEGIN_Tabelle>";
            msg += "\n";
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
            msg += "\n";
        }
    }

    //msg in Datei speichern:

    QString filename;
    filename =  ui->lineEdit_backupto->text();
    filename += QDir::separator();
    filename += FILENAME_TABSTRKT;
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(msg.toUtf8());
        file.close();

        QMessageBox mb;
        mb.setText(tr("Backup erfolgreich abgeschlossen."));
        mb.exec();
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Backup fehlgeschlagen!\nDatei konnte nicht geschrieben werden."));
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
        QApplication::setOverrideCursor((Qt::WaitCursor));
        filetext.set_text(file.readAll());
        //Tabellenstruktur aus der db auslesen:
        text_zeilenweise tables_db = dbeigen->get_tables_tz();

        //Datei auswerten:

        for(uint i=1; i<=filetext.zeilenanzahl() ;i++)
        {
            QString current_table;
            QString zeile = filetext.zeile(i);
            if(zeile.contains("<BEGIN_Tabelle>"))
            {
                i++;
                zeile = filetext.zeile(i);
                QString tablename_csv = zeile;
                tablename_csv.replace("\t","");
                for(uint ii=1; ii<=tables_db.zeilenanzahl() ;ii++)
                {
                    if(tablename_csv.toUpper() == tables_db.zeile(ii).toUpper())
                    {
                        current_table = tablename_csv;
                        break;
                    }
                }



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

                if(!current_table.isEmpty())//Wenn es die Tabelle bereits gibt
                {
                    //Tabelle ggf ergänzen:
                    //bereits vorhandene Param erfassen:
                    text_zeilenweise oldparam = dbeigen->get_param_tz(current_table);

                    bool sindanfangsgleich = true;
                    for(uint ii=1; ii<=oldparam.zeilenanzahl() ;ii++)
                    {
                        if(param.zeile(ii) != oldparam.zeile(ii))
                        {
                            sindanfangsgleich = false;
                            break;
                        }
                    }
                    if(sindanfangsgleich == true)
                    {
                        if(param.zeilenanzahl() > oldparam.zeilenanzahl())
                        {
                            for(uint ii=oldparam.zeilenanzahl()+1; ii<=param.zeilenanzahl() ;ii++)
                            {
                                QString typ = param_type.zeile(ii);
                                QString additional;
                                bool isunsigned = false;
                                if(typ.contains("("))
                                {
                                    additional = text_mitte(typ, "(",")");
                                    if(text_rechts(typ, ")").contains("unsigned"))
                                    {
                                        isunsigned = true;
                                    }
                                    typ = text_links(typ, "(");
                                }
                                bool ispri;
                                if(param_primkey.zeile(ii) == "PRI")
                                {
                                    ispri = true;
                                }else
                                {
                                    ispri = false;
                                }
                                bool isautoincrement;
                                if(param_extra.zeile(ii) == "auto_increment")
                                {
                                    isautoincrement = true;
                                }else
                                {
                                    isautoincrement = false;
                                }
                                bool isnotnull;
                                if(param_notnull.zeile(ii) == "NO")
                                {
                                    isnotnull = true;
                                }else
                                {
                                    isnotnull = false;
                                }
                                QString defaultvalue = param_default.zeile(ii);
                                if(defaultvalue == "---")
                                {
                                    defaultvalue.clear();
                                }
                                dbeigen->param_new(current_table,\
                                                   param.zeile(ii),\
                                                   typ,\
                                                   additional,\
                                                   ispri,\
                                                   isautoincrement,\
                                                   isunsigned,\
                                                   isnotnull,\
                                                   defaultvalue);
                            }                            
                        }

                    }else
                    {
                        QString msg;
                        msg += "Die Tabelle \"";
                        msg += current_table;
                        msg += "\" konnte nicht automatisch abgeglichen werden!";
                        QMessageBox mb;
                        mb.setText(tr(msg.toStdString().c_str()));
                        mb.exec();
                    }
                }else   //Es gibt die Tabelle noch nicht
                {
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
                        mb.setText(tr("Fehler bei Datenbankverbindung!"));
                        mb.exec();
                    }
                    //-------------------------------------
                }
            }else if(zeile.contains("<ENDE_Tabelle>"))
            {
                current_table = -1;
            }
        }
        QApplication::restoreOverrideCursor();
        QMessageBox mb;
        mb.setText(tr("Restore ist fertig"));
        mb.exec();
    }else
    {
        QString msg;
        msg += "Fehler beim Zugriff auf die Datei \"";
        msg += filename;
        msg += "\"!";

        QMessageBox mb;
        mb.setText(tr(msg.toStdString().c_str()));
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

void Form_backup::on_pushButton_backup_all_clicked()
{
    //----------------------------------------------------------
    //Ordner für Backup anlegen:
    QString backupdir = ini->get_rootdir();
    backupdir += QDir::separator();
    backupdir += "backup_manuell";
    backupdir += QDir::separator();

    int y,m,d;
    QDate today = QDate::currentDate();
    today.getDate(&y, &m, &d);
    backupdir += int_to_qstring(y);
    backupdir += "_";
    backupdir += int_to_qstring(m);
    backupdir += "_";
    backupdir += int_to_qstring(d);

    backupdir += "__";

    QTime jetzt = QTime::currentTime();
    int stunde, minute, sekunde;
    stunde = jetzt.hour();
    minute = jetzt.minute();
    sekunde = jetzt.second();
    backupdir += int_to_qstring(stunde);
    backupdir += "_";
    backupdir += int_to_qstring(minute);
    backupdir += "_";
    backupdir += int_to_qstring(sekunde);
    //----------------------------------------------------------
    QString ezpar = "\t";
    text_zeilenweise tables;
    tables = dbeigen->get_tables_tz();
    QApplication::setOverrideCursor((Qt::WaitCursor));
    //----------------------------------------------------------
    //----------------------------------------------------------
    //Komplette tabellenstruktur sichern:
    QString tabstrukt;
    for(uint i=1; i<= tables.zeilenanzahl() ;i++)
    {
        text_zeilenweise param = dbeigen->get_param_tz(tables.zeile(i));
        text_zeilenweise param_type = dbeigen->get_param_type_tz(tables.zeile(i));
        text_zeilenweise param_primkey = dbeigen->get_param_primkey_tz(tables.zeile(i));
        text_zeilenweise param_extra = dbeigen->get_param_extra_tz(tables.zeile(i));
        text_zeilenweise param_notnull = dbeigen->get_param_notnull_tz(tables.zeile(i));
        text_zeilenweise param_default = dbeigen->get_param_default_tz(tables.zeile(i));

        tabstrukt += "<BEGIN_Tabelle>";
        tabstrukt += "\n";
        tabstrukt += tables.zeile(i);
        tabstrukt += "\n";

        tabstrukt += "Field"; //Spaltenname
        tabstrukt += ezpar;
        tabstrukt += "Type";
        tabstrukt += ezpar;
        tabstrukt += "Null";  //Not Null
        tabstrukt += ezpar;
        tabstrukt += "Key";   //Is Primary Key
        tabstrukt += ezpar;
        tabstrukt += "Default"; //Default value
        tabstrukt += ezpar;
        tabstrukt += "Extra";
        tabstrukt += "\n";

        for(uint ii=1; ii<=param.zeilenanzahl() ;ii++)
        {
            tabstrukt += param.zeile(ii);
            tabstrukt += ezpar;
            tabstrukt += param_type.zeile(ii);
            tabstrukt += ezpar;
            tabstrukt += param_notnull.zeile(ii);
            tabstrukt += ezpar;
            tabstrukt += param_primkey.zeile(ii);
            tabstrukt += ezpar;
            tabstrukt += param_default.zeile(ii);
            tabstrukt += ezpar;;
            tabstrukt += param_extra.zeile(ii);
            tabstrukt += ezpar;
            tabstrukt += "\n";
        }

        tabstrukt += "<ENDE_Tabelle>";
        tabstrukt += "\n";
    }
    QString filename;
    filename =  backupdir;
    filename += QDir::separator();
    QDir ordner;
    ordner.mkpath(backupdir);
    filename += FILENAME_TABSTRKT;
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(tabstrukt.toUtf8());
        file.close();
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Backup fehlgeschlagen!\nDatei mit Tabellenstruktur konnte nicht geschrieben werden."));
        mb.exec();
        return;
    }
    //----------------------------------------------------------

    //----------------------------------------------------------
    //Alle Daten sichern:
    for(uint i=1; i<= tables.zeilenanzahl() ;i++)
    {
        QString tabname = tables.zeile(i);
        text_zeilenweise ids = dbeigen->get_data_tz(tabname, "id");
        QString inhalt;
        for(uint ii=1; ii<=ids.zeilenanzahl() ;ii++)
        {
            text_zeilenweise zeile_tz = dbeigen->get_data_zeile_tz(tabname, ids.zeile(ii));
            for(uint iii=1; iii<=zeile_tz.zeilenanzahl() ;iii++)
            {
                inhalt += zeile_tz.zeile(iii);
                if(iii != zeile_tz.zeilenanzahl())
                {
                    inhalt += ezpar;
                }
            }
            if(ii != ids.zeilenanzahl())
            {
                inhalt += "\n";
            }
        }
        filename  =  backupdir;
        filename += QDir::separator();
        filename += tabname;
        filename += ".csv";
        QFile file(filename);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.write(inhalt.toUtf8());
            file.close();
        }else
        {
            QString msg;
            msg += "Backup fehlgeschlagen!\nDatei \"";
            msg += tabname;
            msg += ".csv";
            msg += "\" konnte nicht geschrieben werden.";
            QMessageBox mb;
            mb.setText(msg);
            mb.exec();
            return;
        }
    }
    //----------------------------------------------------------
    QApplication::restoreOverrideCursor();
    QMessageBox mb;
    mb.setText(tr("Backup erfolgreich abgeschlossen."));
    mb.exec();
    //----------------------------------------------------------
}

void Form_backup::on_pushButton_restore_all_clicked()
{

}



























