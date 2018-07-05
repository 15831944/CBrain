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

    db = QSqlDatabase::addDatabase(driver);
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

    db = QSqlDatabase::addDatabase(driver, "noname");
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

    db = QSqlDatabase();
    db.removeDatabase("noname");

    return pingok;
}

text_zeilenweise cbrainbatabase::get_tables_tz()
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::addDatabase(driver, "noname");
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

    db = QSqlDatabase();
    db.removeDatabase("noname");

    return tz;
}

text_zeilenweise cbrainbatabase::get_table_head(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::addDatabase(driver, "noname");
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

    db = QSqlDatabase();
    db.removeDatabase("noname");
    return tz;
}

text_zeilenweise cbrainbatabase::get_table_head_type(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::addDatabase(driver, "noname");
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

    db = QSqlDatabase();
    db.removeDatabase("noname");
    return tz;
}

text_zeilenweise cbrainbatabase::get_table_is_primary_key(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::addDatabase(driver, "noname");
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
                tz.zeile_anhaengen(q.value(3).toString());
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

    db = QSqlDatabase();
    db.removeDatabase("noname");
    return tz;
}

text_zeilenweise cbrainbatabase::get_table_value_extra(QString tablename)
{
    text_zeilenweise tz;

    QSqlDatabase db;

    db = QSqlDatabase::addDatabase(driver, "noname");
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
                tz.zeile_anhaengen(q.value(5).toString());
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

    db = QSqlDatabase();
    db.removeDatabase("noname");
    return tz;
}





















