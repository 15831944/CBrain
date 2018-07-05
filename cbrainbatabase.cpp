#include "cbrainbatabase.h"

cbrainbatabase::cbrainbatabase()
{
    clear();
}

cbrainbatabase::~cbrainbatabase()
{
    clear();
}

void cbrainbatabase::clear()
{
    close();
    host.clear();
    dbname.clear();
    driver.clear();
    user.clear();
    pwd.clear();
}

bool cbrainbatabase::open()
{
    if(isopen == true)
    {
        close();
        open(); //rekusiv
    }else
    {
        db = QSqlDatabase::addDatabase(driver);
        db.setHostName(host);
        db.setDatabaseName(dbname);
        db.setUserName(user);
        db.setPassword(pwd);
        isopen = db.open();
    }

    return isopen;
}

void cbrainbatabase::close()
{
    db.close();
    isopen = false;
}

void cbrainbatabase::set_param(text_zeilenweise param)
{
    if(isopen == true)
    {
        close();
    }
    set_host(param.zeile(1));
    set_dbname(param.zeile(2));
    set_driver(param.zeile(3));
    set_user(param.zeile(4));
    set_pwd(param.zeile(5));
}

text_zeilenweise cbrainbatabase::get_tables_tz()
{
    text_zeilenweise tz;

    if(isopen == true)
    {
        tz.set_text(db.tables());
        QMessageBox mb;
        mb.setText("war offen");
        mb.exec();
    }else
    {
        if(open())
        {
            tz.set_text(db.tables());
            close();
        }else
        {
            QMessageBox mb;
            mb.setText("Fehler bei Datenbankverbindung!");
            mb.exec();
        }

    }

    return tz;
}
