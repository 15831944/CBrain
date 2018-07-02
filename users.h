#ifndef USERS_H
#define USERS_H

#define USERADMIN_NAME      "CERA"
#define USERADMIN_PWD       "CERA"

#include <QString>
#include <QMessageBox>
#include "text_zeilenweise.h"

class users
{
public:
    users();
    users(QString users);
    void set_users(QString users);
    QString get_users();

    bool login(QString user, QString pwd);
    bool is_admin();


private:
    QString trzparam;               //Trennzeichen der einzelnen Parameter eines Nutzers
    char    trz_char;
    text_zeilenweise user_tz;       //Lister der Nutzer
    text_zeilenweise pwd_tz;        //Lister der Passwörter
    text_zeilenweise is_admin_tz;   //Liste welcher User Admin-Rechte hat
                                    // 1 == hat Rechte  |  0 == hat keine Rechte
    uint current_user;


    void clear();
};

#endif // USERS_H
