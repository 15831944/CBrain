#ifndef CBRAINBATABASE_H
#define CBRAINBATABASE_H

#include <QtSql>
#include <QString>
#include <QMessageBox>
#include "text_zeilenweise.h"
#include "umwandeln.h"

#define DBEMPTYFIELD    "---"

//table == Tabelle
//param == Überschrif == mehrere param bilden den Tabellenkopf
//data == eine Datenzeile in der Tabelle
//value == Werte == der Inhalt eines Datenfeldes in einer Zeile


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

    //------------------------------------------
    //-------------------------------table:
    bool table_new(QString tablename);
    bool table_del(QString tablename);
    bool table_rename(QString tablename_old, QString tablename_new);
    //----------------
    text_zeilenweise get_tables_tz();
    //------------------------------------------
    //-------------------------------param:
    bool param_new(QString tablename, QString paramname, \
                   QString typ, QString additional, \
                   bool ispri, bool autoincrement, bool isunsigned,\
                   bool notnull, QString defaultvalue);
    bool param_del(QString tablename, QString parmname);
    bool param_edit(QString tablename, QString paramname_old, QString paramname_new,\
                    QString typ, QString additional, \
                    bool ispri, bool autoincrement, bool isunsigned,\
                    bool notnull, QString defaultvalue);
    //----------------
    text_zeilenweise get_param_tz(QString tablename);
    text_zeilenweise get_param_type_tz(QString tablename);
    text_zeilenweise get_param_primkey_tz(QString tablename);
    text_zeilenweise get_param_extra_tz(QString tablename);
    text_zeilenweise get_param_notnull_tz(QString tablename);
    text_zeilenweise get_param_default_tz(QString tablename);
    //------------------------------------------
    //-------------------------------data:
    bool data_new(QString tablename, text_zeilenweise param, text_zeilenweise values);
    bool data_new(QString tablename);
    bool data_del(QString tablename, text_zeilenweise ids);
    bool data_del(QString tablename, QString id);
    bool data_edit(QString tablename, QString param, QString value, QString id);
    bool data_edit(QString tablename, text_zeilenweise param, text_zeilenweise value, QString id);
    //----------------
    QString get_data_qstring(QString tablename, QString param, QString id);
    QString get_data_qstring(QString tablennameA, QString tabA_param_with_id_of_B, QString tabA_id, \
                             QString tablenameB, QString tabB_param_for_return);
    text_zeilenweise get_data_tz(QString tablename, QString param);
    QString get_highest_id(QString tablename);
    QString get_data_id_QString(QString tablename, QString param, QString value);
    text_zeilenweise get_data_id_tz(QString tablename, QString param, QString value);
    //------------------------------------------
    //-------------------------------values:
    text_zeilenweise get_values_from_column(QString tablename, int column, QString querryfilter);


private:
    QString host;
    QString dbname;
    QString driver;
    QString user;
    QString pwd;




};

#endif // CBRAINBATABASE_H
