#ifndef CBRAINBATABASE_H
#define CBRAINBATABASE_H

#include <QtSql>
#include <QString>
#include <QMessageBox>
#include "text_zeilenweise.h"
#include "umwandeln.h"

class cbrainbatabase
{
public:
    cbrainbatabase();
    ~cbrainbatabase();
    void set_param(text_zeilenweise param);

    void clear();

    inline void set_host(QString new_host)
    {
        host = new_host;
    }
    inline QString get_host()
    {
        return host;
    }
    inline void set_driver(QString new_driver)
    {
        driver = new_driver;
    }
    inline QString get_driver()
    {
        return driver;
    }
    inline void set_dbname(QString new_dbname)
    {
        dbname = new_dbname;
    }
    inline QString get_dbname()
    {
        return dbname;
    }
    inline void set_user(QString new_user)
    {
        user = new_user;
    }
    inline QString get_user()
    {
        return user;
    }
    inline void set_pwd(QString new_pwd)
    {
        pwd = new_pwd;
    }
    inline QString get_pwd()
    {
        return pwd;
    }


    bool pingdb();

    text_zeilenweise get_tables_tz();
    text_zeilenweise get_table_head(QString tablename);
    text_zeilenweise get_table_head_type(QString tablename);
    text_zeilenweise get_table_is_primary_key(QString tablename);
    text_zeilenweise get_table_value_extra(QString tablename);


private:
    QString host;
    QString dbname;
    QString driver;
    QString user;
    QString pwd;




};

#endif // CBRAINBATABASE_H
