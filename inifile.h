#ifndef INIFILE_H
#define INIFILE_H

#include <QString>
#include "text_zeilenweise.h"

class inifile
{
public:
    inifile();
    void clear();

    void set_text(QString text);
    QString get_text();

    inline void set_rootdir(QString dir)
    {
        rootdir = dir;
    }
    inline QString get_rootdir()
    {
        return rootdir;
    }

    text_zeilenweise get_settings_db_eigen();
    void set_settings_db_eigen(text_zeilenweise data);

    inline void set_modul_backup_to(QString path)
    {
        settings_modul_backup_to = path;
    }
    inline QString get_modul_backup_to()
    {
        return settings_modul_backup_to;
    }
    inline void set_modul_backup_from(QString path)
    {
        settings_modul_backup_from = path;
    }
    inline QString get_modul_backup_from()
    {
        return settings_modul_backup_from;
    }

private:
    QString rootdir;

    QString settings_db_eigen_hostname;
    QString settings_db_eigen_dbname;
    QString settings_db_eigen_driver;
    QString settings_db_eigen_username;
    QString settings_db_eigen_userpwd;

    QString settings_modul_backup_to;
    QString settings_modul_backup_from;


};

#endif // INIFILE_H
