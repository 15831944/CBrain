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
    void removeuser(uint index);
    uint get_anz_admins();

    bool login(QString user, QString pwd);
    bool is_admin();
    bool is_admin(uint index);
    inline text_zeilenweise get_names_tz()
    {
        return user_tz;
    }
    inline text_zeilenweise get_pwd_tz()
    {
        return pwd_tz;
    }
    inline text_zeilenweise get_isadmin_tz()
    {
        return is_admin_tz;
    }
    bool change_name(uint index, QString newname);
    void change_pwd(uint index, QString newpwd);
    void change_isadmin(uint index, bool isadmin);

private:
    QString trzparam;               //Trennzeichen der einzelnen Parameter eines Nutzers
    char    trz_char;
    text_zeilenweise user_tz;       //Lister der Nutzer
    text_zeilenweise pwd_tz;        //Lister der Passwörter
    text_zeilenweise is_admin_tz;   //Liste welcher User Admin-Rechte hat
                                    // 1 == hat Rechte  |  0 == hat keine Rechte
    uint current_user;


    void clear();
    void set_first_admin();
};

#endif // USERS_H
