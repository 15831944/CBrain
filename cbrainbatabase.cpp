#include "cbrainbatabase.h"

cbrainbatabase::cbrainbatabase()
{
    clear();
}

cbrainbatabase::~cbrainbatabase()
{
    clear();
}

/*
QSqlDatabase cbrainbatabase::get_db()
{
    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    return db;
}
*/

void cbrainbatabase::clear()
{
    host.clear();
    dbname.clear();
    driver.clear();
    user.clear();
    pwd.clear();
}

void cbrainbatabase::set_param(text_zeilenweise param)
{
    set_host(param.zeile(1));
    set_dbname(param.zeile(2));
    set_driver(param.zeile(3));
    set_user(param.zeile(4));
    set_pwd(param.zeile(5));
}

bool cbrainbatabase::pingdb()
{
    bool pingok;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);


    if(db.open())
    {
        pingok = true;
        db.close();
    }else
    {
        pingok = false;
    }

    return pingok;
}

//------------------------------------------
//-------------------------------table:
bool cbrainbatabase::table_new(QString tablename)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "CREATE TABLE ";
        cmd += tablename;
        cmd += "(id int(11) unsigned auto_increment primary key not null)";
        cmd += " ENGINE=InnoDB";

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

bool cbrainbatabase::table_del(QString tablename)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "DROP TABLE ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

bool cbrainbatabase::table_rename(QString tablename_old, QString tablename_new)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "ALTER TABLE ";
        cmd += tablename_old;
        cmd += " RENAME ";
        cmd += tablename_new;

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

//----------------
text_zeilenweise cbrainbatabase::get_tables_tz()
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal"); //Diese Zeile verursacht folgende Fehlermeldung:
    //QSqlDatabasePrivate::database: unable to open database: "Access denied for user 'oliver'@'localhost' (using password: NO) QMYSQL: Unable to connect"
    //Aber warum?
    //Die DB kann jedoch scheinbar ohne Einschränkungen geöffnen und ausgelesen werden
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        tz.set_text(db.tables());
        db.close();
    }else
    {
        QMessageBox mb;
        mb.setText("Fehler bei Datenbankverbindung!");
        mb.exec();
    }

    return tz;
}

//------------------------------------------
//-------------------------------param:
bool cbrainbatabase::param_new(QString tablename, QString paramname, QString typ, QString additional, \
                               bool ispri, bool autoincrement, bool isunsigned,\
                               bool notnull, QString defaultvalue)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "ALTER TABLE ";
        cmd += tablename;
        cmd += " ADD COLUMN (";
        cmd += paramname;
        cmd += " ";
        cmd += typ;
        if(!additional.isEmpty())
        {
            cmd += "(";
            cmd += additional;
            cmd += ")";
        }
        if(isunsigned == true)
        {
            cmd += " unsigned";
        }
        if(autoincrement == true)
        {
            cmd += " auto_increment";
        }
        if(ispri == true)
        {
            cmd += " primary key";
        }
        if(notnull == true)
        {
            cmd += " not null";
        }
        if(!defaultvalue.isEmpty())
        {
            cmd += " DEFAULT \'";
            cmd += defaultvalue;
            cmd += "\'";
        }

        cmd += ")";

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

bool cbrainbatabase::param_del(QString tablename, QString parmname)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "ALTER TABLE ";
        cmd += tablename;
        cmd += " DROP COLUMN ";
        cmd += parmname;

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

bool cbrainbatabase::param_edit(QString tablename, QString paramname_old, QString paramname_new, QString typ, QString additional, \
                                bool ispri, bool autoincrement, bool isunsigned,\
                                bool notnull, QString defaultvalue)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "ALTER TABLE ";
        cmd += tablename;
        cmd += " CHANGE ";
        cmd += paramname_old;
        cmd += " ";
        cmd += paramname_new;
        cmd += " ";
        cmd += typ;
        if(!additional.isEmpty())
        {
            cmd += "(";
            cmd += additional;
            cmd += ")";
        }
        if(isunsigned == true)
        {
            cmd += " unsigned";
        }
        if(autoincrement == true)
        {
            cmd += " auto_increment";
        }
        if(ispri == true)
        {
            cmd += " primary key";
        }
        if(notnull == true)
        {
            cmd += " not null";
        }
        if(!defaultvalue.isEmpty())
        {
            cmd += " DEFAULT \'";
            cmd += defaultvalue;
            cmd += "\'";
        }

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

//----------------
text_zeilenweise cbrainbatabase::get_param_tz(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SHOW COLUMNS FROM ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                tz.zeile_anhaengen(q.value(0).toString());
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

    return tz;
}

text_zeilenweise cbrainbatabase::get_param_type_tz(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SHOW COLUMNS FROM ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                tz.zeile_anhaengen(q.value(1).toString());
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

    return tz;
}

text_zeilenweise cbrainbatabase::get_param_primkey_tz(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SHOW COLUMNS FROM ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                QString tmp = q.value(3).toString();
                if(!tmp.isEmpty())
                {
                    tz.zeile_anhaengen(tmp);
                }else
                {
                    tz.zeile_anhaengen("---");
                }
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

    return tz;
}

text_zeilenweise cbrainbatabase::get_param_extra_tz(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SHOW COLUMNS FROM ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                QString tmp = q.value(5).toString();
                if(!tmp.isEmpty())
                {
                    tz.zeile_anhaengen(tmp);
                }else
                {
                    tz.zeile_anhaengen("---");
                }
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

    return tz;
}

text_zeilenweise cbrainbatabase::get_param_notnull_tz(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SHOW COLUMNS FROM ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                QString tmp = q.value(2).toString();
                if(!tmp.isEmpty())
                {
                    tz.zeile_anhaengen(tmp);
                }else
                {
                    tz.zeile_anhaengen("---");
                }
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

    return tz;
}

text_zeilenweise cbrainbatabase::get_param_default_tz(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SHOW COLUMNS FROM ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                QString tmp = q.value(4).toString();
                if(!tmp.isEmpty())
                {
                    tz.zeile_anhaengen(tmp);
                }else
                {
                    tz.zeile_anhaengen("---");
                }
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

    return tz;
}

//------------------------------------------
//-------------------------------data:
bool cbrainbatabase::data_new(QString tablename, text_zeilenweise param, text_zeilenweise values)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "INSERT INTO ";
        cmd += tablename;
        cmd += " (";
        for(uint i=1; i<=param.zeilenanzahl() ;i++)
        {
            cmd += param.zeile(i);
            if(i!=param.zeilenanzahl())
            {
                cmd += ", ";
            }
        }
        cmd += ") VALUES (\'";
        for(uint i=1; i<=values.zeilenanzahl() ;i++)
        {
            cmd += values.zeile(i);
            if(i!=values.zeilenanzahl())
            {
                cmd += "\', \'";
            }
        }
        cmd += "\')";

        if(q.exec(cmd))
        {
            ok = true;
        }else
        {
            QMessageBox mb;
            mb.setText("Fehler:\n" + q.lastError().text());
            mb.exec();
            ok = false;
        }
        db.close();

    }else
    {
        QMessageBox mb;
        mb.setText("Fehler bei Datenbankverbindung!");
        mb.exec();
    }
    return ok;
}
bool cbrainbatabase::data_new(QString tablename)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "INSERT INTO ";
        cmd += tablename;
        cmd += " VALUES ()";

        if(q.exec(cmd))
        {
            ok = true;
        }else
        {
            QMessageBox mb;
            mb.setText("Fehler:\n" + q.lastError().text());
            mb.exec();
            ok = false;
        }
        db.close();

    }else
    {
        QMessageBox mb;
        mb.setText("Fehler bei Datenbankverbindung!");
        mb.exec();
    }
    return ok;
}

bool cbrainbatabase::data_del(QString tablename, text_zeilenweise ids)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        for(uint i=1; i<=ids.zeilenanzahl() ;i++)
        {
            QSqlQuery q(db);
            QString cmd;
            cmd += "DELETE FROM ";
            cmd += tablename;
            cmd += " WHERE id = \'";
            cmd += ids.zeile(i);
            cmd += "\'";

            if(q.exec(cmd))
            {
                ok = true;
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
                ok = false;
                break;
            }
        }
        db.close();

    }else
    {
        QMessageBox mb;
        mb.setText("Fehler bei Datenbankverbindung!");
        mb.exec();
    }
    return ok;
}
bool cbrainbatabase::data_del(QString tablename, QString id)
{
    text_zeilenweise tz;
    tz.set_text(id);
    return data_del(tablename, tz);
}

bool cbrainbatabase::data_edit(QString tablename, QString param, \
                                 QString value, QString id)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "UPDATE ";
        cmd += tablename;
        cmd += " SET ";
        cmd += param;
        cmd += " = ";
        if(value.isEmpty())
        {
            cmd += "NULL";
        }else
        {
            cmd += "\'";
            cmd += value;
            cmd += "\'";
        }
        cmd += " WHERE ID = \'";
        cmd += id;
        cmd += "\'";

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

bool cbrainbatabase::data_edit(QString tablename, text_zeilenweise param, \
                                 text_zeilenweise value, QString id)
{
    bool ok = false;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "UPDATE ";
        cmd += tablename;
        cmd += " SET ";
        for(uint i=1; i<=param.zeilenanzahl() ;i++)
        {
            cmd += param.zeile(i);
            cmd += " = \'";
            cmd += value.zeile(i);
            cmd += "\'";
            if(i < param.zeilenanzahl())
            {
                cmd += ", ";
            }
        }
        cmd += " WHERE ID = ";
        cmd += id;

        if(q.exec(cmd))
        {
            ok = true;
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
    return ok;
}

//----------------
QString cbrainbatabase::get_data_qstring(QString tablename, QString param, QString id)
{
    QString msg;
    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SELECT ";
        cmd += param;
        cmd += " FROM ";
        cmd += tablename;
        cmd += " WHERE id=\'";
        cmd += id;
        cmd += "\'";

        if(q.exec(cmd))
        {
            while(q.next())
            {
                msg += q.value(0).toString();
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
    return msg;
}

QString cbrainbatabase::get_data_qstring(QString tablennameA, QString tabA_param_with_id_of_B, QString tabA_id, \
                                         QString tablenameB, QString tabB_param_for_return)
{
    //Beispiel:
    //tablenameA = Artikel, tabA_param_with_id_of_B = Block durch user(id),
    QString tabB_id = get_data_qstring(tablennameA, tabA_param_with_id_of_B, tabA_id);
    return get_data_qstring(tablenameB, tabB_param_for_return, tabB_id);
}

text_zeilenweise cbrainbatabase::get_data_tz(QString tablename, QString param)
{
    text_zeilenweise tz;
    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SELECT ";
        cmd += param;
        cmd += " FROM ";
        cmd += tablename;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                tz.zeile_anhaengen(q.value(0).toString());
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
    return tz;
}

text_zeilenweise cbrainbatabase::get_data_zeile_tz(QString tablename, QString id)
{
    text_zeilenweise tz;
    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SELECT *";
        cmd += " FROM ";
        cmd += tablename;
        cmd += " WHERE ";
        cmd += "id=";
        cmd += id;

        if(q.exec(cmd))
        {
            while(q.next())
            {
                QSqlRecord rec = q.record();
                for(int i=0; i<rec.count() ;i++)
                {
                    tz.zeile_anhaengen(rec.value(i).toString());
                }
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
    return tz;
}

QString cbrainbatabase::get_highest_id(QString tablename)
{
    text_zeilenweise tz = get_data_tz(tablename, "id");//Achtung id muss auch immer "id" heißen damit es keine Probleme gibt!
    return tz.zeile(tz.zeilenanzahl());
}

QString cbrainbatabase::get_data_id_QString(QString tablename, QString param, QString value)
{
    QString id = "";
    text_zeilenweise ids = get_data_tz(tablename, "id");
    text_zeilenweise values = get_data_tz(tablename, param);
    for(uint i=1; i<=ids.zeilenanzahl() ;i++)
    {
        if(values.zeile(i) == value)
        {
            id = ids.zeile(i);
            break;
        }
    }
    return id;
}

text_zeilenweise cbrainbatabase::get_data_id_tz(QString tablename, QString param, QString value)
{
    text_zeilenweise return_ids;
    text_zeilenweise ids = get_data_tz(tablename, "id");
    text_zeilenweise values = get_data_tz(tablename, param);
    for(uint i=1; i<=ids.zeilenanzahl() ;i++)
    {
        if(values.zeile(i) == value)
        {
            return_ids.zeile_anhaengen(ids.zeile(i));
        }
    }
    return return_ids;
}

//------------------------------------------
//-------------------------------values:
text_zeilenweise cbrainbatabase::get_values_from_column(QString tablename,  int column, QString querryfilter)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::database("dbglobal");
    db.setHostName(host);
    db.setDatabaseName(dbname);
    db.setUserName(user);
    db.setPassword(pwd);

    if(db.open())
    {
        QSqlQuery q(db);
        QString cmd;
        cmd += "SELECT * FROM ";
        cmd += tablename;
        if(!querryfilter.isEmpty())
        {
            cmd += " WHERE ";
            cmd += querryfilter;
        }

        if(q.exec(cmd))
        {
            while(q.next())
            {
                QString tmp = q.value(column).toString();
                if(tmp.isEmpty())
                {
                    tz.zeile_anhaengen(DBEMPTYFIELD);
                }else
                {
                    tz.zeile_anhaengen(tmp);
                }
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

    return tz;
}


