#include "inifile.h"

inifile::inifile()
{
    clear();
}

void inifile::clear()
{
    rootdir = "";
}

void inifile::set_text(QString text)
{
    text_zeilenweise tz;
    tz.set_text(text);

    set_rootdir(tz.zeile(1));
    settings_db_eigen_hostname = tz.zeile(2);
    settings_db_eigen_dbname = tz.zeile(3);
    settings_db_eigen_driver = tz.zeile(4);
    settings_db_eigen_username = tz.zeile(5);
    settings_db_eigen_userpwd = tz.zeile(6);
}

QString inifile::get_text()
{
    QString msg;

    msg += get_rootdir();                   //Zeile  1
    msg += "\n";
    msg += settings_db_eigen_hostname;      //Zeile  2
    msg += "\n";
    msg += settings_db_eigen_dbname;        //Zeile  3
    msg += "\n";
    msg += settings_db_eigen_driver;        //Zeile  4
    msg += "\n";
    msg += settings_db_eigen_username;      //Zeile  5
    msg += "\n";
    msg += settings_db_eigen_userpwd;       //Zeile  6

    return msg;
}

text_zeilenweise inifile::get_settings_db_eigen()
{
    text_zeilenweise tz;
    tz.zeile_anhaengen(settings_db_eigen_hostname);     //Zeile  1
    tz.zeile_anhaengen(settings_db_eigen_dbname);       //Zeile  2
    tz.zeile_anhaengen(settings_db_eigen_driver);       //Zeile  3
    tz.zeile_anhaengen(settings_db_eigen_username);     //Zeile  4
    tz.zeile_anhaengen(settings_db_eigen_userpwd);      //Zeile  5

    return tz;
}

void inifile::set_settings_db_eigen(text_zeilenweise data)
{
     settings_db_eigen_hostname = data.zeile(1);
     settings_db_eigen_dbname = data.zeile(2);
     settings_db_eigen_driver = data.zeile(3);
     settings_db_eigen_username = data.zeile(4);
     settings_db_eigen_userpwd = data.zeile(5);
}
