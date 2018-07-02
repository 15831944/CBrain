#include "users.h"

users::users()
{
    clear();
}

users::users(QString users)
{
    set_users(users);
}

void users::set_users(QString users)
{
    clear();
    text_zeilenweise input;
    input.set_text(users);
    text_zeilenweise zeile;
    zeile.set_trennzeichen(trz_char);

    //Achtung! Wenn die Parameter für den Nutzer erweitert werden auch
    //clear() erweitern --> Wartungspasswort

    //Benutzer einlesen:
    for(uint i=1; i<=input.zeilenanzahl() ;i++)
    {
        zeile.set_text(input.zeile(i));
        user_tz.zeile_anhaengen(zeile.zeile(1));
        pwd_tz.zeile_anhaengen(zeile.zeile(2));
        is_admin_tz.zeile_anhaengen(zeile.zeile(3));
    }
}

QString users::get_users()
{
    QString msg;

    for(uint i =1; i<=user_tz.zeilenanzahl() ;i++)
    {
        msg += user_tz.zeile(i);
        msg += trzparam;
        msg += pwd_tz.zeile(i);
        msg += trzparam;
        msg += is_admin_tz.zeile(i);

        if(i != user_tz.zeilenanzahl()) //in der letzten Zeile keinen Zeilenvorschub
        {
            msg += "\n";
        }
    }

    return msg;
}

void users::clear()
{
    trzparam = "\t";    //Nur 1 Zeichen !!
    trz_char = '\t';
    user_tz.clear();
    pwd_tz.clear();
    is_admin_tz.clear();
    current_user = 0;

    //Wartungspasswort:
    user_tz.zeile_anhaengen(USERADMIN_NAME);
    pwd_tz.zeile_anhaengen(USERADMIN_PWD);
    is_admin_tz.zeile_anhaengen("1");
}

bool users::login(QString user, QString pwd)
{
    uint tmp = 0;
    for(uint i=1; i<=user_tz.zeilenanzahl() ;i++)
    {
        if(user_tz.zeile(i) == user     &&\
           pwd_tz.zeile(i)  == pwd      )
        {
            tmp = i;
            break;
        }
    }
    if(tmp != 0)
    {
        current_user = tmp;
        return true;        //erfolgreich angemeldet
    }else
    {
        return false;       //Anmeldung schlug fehl
    }
}

bool users::is_admin()
{
    if(current_user != 0)
    {
        if(is_admin_tz.zeile(current_user) == "1")
        {
            return true;
        }else
        {
            return false;
        }
    }else
    {
        return false;
    }
}
