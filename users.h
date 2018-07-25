#ifndef USERS_H
#define USERS_H

#define USERADMIN_NAME      "CERA"
#define USERADMIN_PWD       "CERA"

#include <QString>
#include <QMessageBox>
#include "text_zeilenweise.h"
#include "umwandeln.h"

class users
{
public:
    users();
    users(QString users);
    void set_users(QString users);
    void newuser();
    QString get_users();
    QString get_current_user();
    QString get_current_user_id();
    void removeuser(uint index);
    uint get_anz_admins();

    bool login(QString user, QString pwd);
    bool is_admin();
    bool is_admin(uint index);
    bool modul_artikel();
    bool modul_lieferanten();
    bool modul_lager();
    bool modul_projekte();
    bool modul_personal();

    inline text_zeilenweise get_names_tz()
    {
        return user_tz;
    }
    inline text_zeilenweise get_pwd_tz()
    {
        return pwd_tz;
    }
    inline text_zeilenweise get_id_tz()
    {
        return id_tz;
    }
    inline text_zeilenweise get_isadmin_tz()
    {
        return is_admin_tz;
    }
    inline text_zeilenweise get_use_modul_artikel_tz()
    {
        return use_modul_artikel;
    }
    inline text_zeilenweise get_use_modul_lieferanten_tz()
    {
        return use_modul_lieferanten;
    }
    inline text_zeilenweise get_use_modul_lager_tz()
    {
        return use_modul_lager;
    }
    inline text_zeilenweise get_use_modul_projekte_tz()
    {
        return use_modul_projekte;
    }
    inline text_zeilenweise get_use_modul_personal_tz()
    {
        return use_modul_personal;
    }

    bool change_name(uint index, QString newname);
    void change_pwd(uint index, QString newpwd);
    void change_id(uint index, QString newid);
    void change_isadmin(uint index, bool isadmin);
    void change_use_modul_artikel(uint index, bool isalowed);
    void change_use_modul_lieferanten(uint index, bool isalowed);
    void change_use_modul_lager(uint index, bool isalowed);
    void change_use_modul_projekte(uint index, bool isalowed);
    void change_use_modul_personal(uint index, bool isalowed);

private:
    QString trzparam;               //Trennzeichen der einzelnen Parameter eines Nutzers
    char    trz_char;
    text_zeilenweise user_tz;       //Lister der Nutzer
    text_zeilenweise pwd_tz;        //Lister der Passwörter
    text_zeilenweise id_tz;         //ID des Nutzers in der DB im Modul Personal
    text_zeilenweise is_admin_tz;   //Liste welcher User Admin-Rechte hat
                                    // 1 == hat Rechte  |  0 == hat keine Rechte
    text_zeilenweise use_modul_artikel;     //Liste Welcher User das Modul Artikel nutzen darf 0|1
    text_zeilenweise use_modul_lieferanten;
    text_zeilenweise use_modul_lager;
    text_zeilenweise use_modul_projekte;
    text_zeilenweise use_modul_personal;



    uint current_user;


    void clear();
    void set_first_admin();
};

#endif // USERS_H
