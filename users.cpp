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
    set_first_admin();

    //Benutzer einlesen:
    for(uint i=1; i<=input.zeilenanzahl() ;i++)
    {
        zeile.set_text(input.zeile(i));
        if(zeile.zeile(1) != USERADMIN_NAME)
        {
            user_tz.zeile_anhaengen(zeile.zeile(1));                //Wert 1
            pwd_tz.zeile_anhaengen(zeile.zeile(2));                 //Wert 2
            is_admin_tz.zeile_anhaengen(zeile.zeile(3));            //Wert 3
            use_modul_artikel.zeile_anhaengen(zeile.zeile(4));      //Wert 4
            use_modul_lieferanten.zeile_anhaengen(zeile.zeile(5));  //Wert 5
            use_modul_lager.zeile_anhaengen(zeile.zeile(6));        //Wert 6
            use_modul_projekte.zeile_anhaengen(zeile.zeile(7));     //Wert 7
            id_tz.zeile_anhaengen(zeile.zeile(8));                  //Wert 8
            use_modul_personal.zeile_anhaengen(zeile.zeile(9));     //Wert 9
            use_modul_matlist.zeile_anhaengen(zeile.zeile(10));     //Wert 10
            use_function_artfavsort.zeile_anhaengen(zeile.zeile(11));//Wert 11
            use_function_projektdel.zeile_anhaengen(zeile.zeile(12));//Wert 12
            use_modul_bestellungen.zeile_anhaengen(zeile.zeile(13)); //Wert 13
        }
    }
}

QString users::get_users()
{
    QString msg;

    for(uint i =1; i<=user_tz.zeilenanzahl() ;i++)
    {
        msg += user_tz.zeile(i);                //Wert 1
        msg += trzparam;
        msg += pwd_tz.zeile(i);                 //Wert 2
        msg += trzparam;
        msg += is_admin_tz.zeile(i);            //Wert 3
        msg += trzparam;
        msg += use_modul_artikel.zeile(i);      //Wert 4
        msg += trzparam;
        msg += use_modul_lieferanten.zeile(i);  //Wert 5
        msg += trzparam;
        msg += use_modul_lager.zeile(i);        //Wert 6
        msg += trzparam;
        msg += use_modul_projekte.zeile(i);     //Wert 7
        msg += trzparam;
        msg += id_tz.zeile(i);                  //Wert 8
        msg += trzparam;
        msg += use_modul_personal.zeile(i);     //Wert 9
        msg += trzparam;
        msg += use_modul_matlist.zeile(i);      //Wert 10
        msg += trzparam;
        msg += use_function_artfavsort.zeile(i);//Wert 11
        msg += trzparam;
        msg += use_function_projektdel.zeile(i);//Wert 12
        msg += trzparam;
        msg += use_modul_bestellungen.zeile(i);//Wert 13

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
    use_modul_artikel.clear();
    use_modul_lieferanten.clear();
    use_modul_lager.clear();
    use_modul_projekte.clear();
    id_tz.clear();
    use_modul_personal.clear();
    use_modul_matlist.clear();
    use_function_artfavsort.clear();
    use_function_projektdel.clear();
    use_modul_bestellungen.clear();
}

void users::set_first_admin()
{
    //Wartungspasswort:
    user_tz.zeile_anhaengen(USERADMIN_NAME);
    pwd_tz.zeile_anhaengen(USERADMIN_PWD);
    is_admin_tz.zeile_anhaengen("1");
    use_modul_artikel.zeile_anhaengen("1");
    use_modul_lieferanten.zeile_anhaengen("1");
    use_modul_lager.zeile_anhaengen("1");
    use_modul_projekte.zeile_anhaengen("1");
    use_modul_personal.zeilen_anhaengen("1");
    id_tz.zeilen_anhaengen("1");                //der Admin muss die id 1 in der Tabelle haben!!
    use_modul_matlist.zeilen_anhaengen("1");
    use_function_artfavsort.zeilen_anhaengen("1");
    use_function_projektdel.zeilen_anhaengen("1");
    use_modul_bestellungen.zeilen_anhaengen("1");
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

QString users::get_current_user()
{
    if(current_user != 0)
    {
        return user_tz.zeile(current_user);
    }else
    {
        return "nobody";
    }
}

QString users::get_current_user_id()
{
    if(current_user != 0)
    {
        return id_tz.zeile(current_user);
    }else
    {
        return "0";
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
bool users::is_admin(uint index)
{
    if(index <= is_admin_tz.zeilenanzahl())
    {
        if(is_admin_tz.zeile(index) == "1")
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

bool users::modul_artikel()
{
    if(current_user != 0)
    {
        if(use_modul_artikel.zeile(current_user) == "1")
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
bool users::modul_lieferanten()
{
    if(current_user != 0)
    {
        if(use_modul_lieferanten.zeile(current_user) == "1")
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
bool users::modul_lager()
{
    if(current_user != 0)
    {
        if(use_modul_lager.zeile(current_user) == "1")
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
bool users::modul_projekte()
{
    if(current_user != 0)
    {
        if(use_modul_projekte.zeile(current_user) == "1")
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
bool users::modul_personal()
{
    if(current_user != 0)
    {
        if(use_modul_personal.zeile(current_user) == "1")
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
bool users::modul_matlist()
{
    if(current_user != 0)
    {
        if(use_modul_matlist.zeile(current_user) == "1")
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
bool users::function_artfavsort()
{
    if(current_user != 0)
    {
        if(use_function_artfavsort.zeile(current_user) == "1")
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
bool users::function_projektdel()
{
    if(current_user != 0)
    {
        if(use_function_projektdel.zeile(current_user) == "1")
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
bool users::modul_bestellungen()
{
    if(current_user != 0)
    {
        if(use_modul_bestellungen.zeile(current_user) == "1")
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

bool users::change_name(uint index, QString newname)
{
    //Prüfen, ob sich der Name überhaupt geänder hat:
    if(user_tz.zeile(index) == newname)
    {
        return true;
    }

    //Prüfen ob Name bereits verwendet wird:
    bool isvalid = true;
    for(uint i=1; i<=user_tz.zeilenanzahl() ;i++)
    {
        if(user_tz.zeile(i) == newname)
        {
            isvalid = false;
            break;
        }
    }
    if(isvalid == true)
    {
        user_tz.zeile_ersaetzen(index, newname);
    }
    return isvalid;
}
void users::change_pwd(uint index, QString newpwd)
{
    pwd_tz.zeile_ersaetzen(index, newpwd);
}
void users::change_id(uint index, QString newid)
{
    id_tz.zeile_ersaetzen(index, newid);
}
void users::change_isadmin(uint index, bool isadmin)
{
    if(isadmin == true)
    {
        is_admin_tz.zeile_ersaetzen(index, "1");
    }else
    {
        is_admin_tz.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_modul_artikel(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_modul_artikel.zeile_ersaetzen(index, "1");
    }else
    {
        use_modul_artikel.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_modul_lieferanten(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_modul_lieferanten.zeile_ersaetzen(index, "1");
    }else
    {
        use_modul_lieferanten.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_modul_lager(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_modul_lager.zeile_ersaetzen(index, "1");
    }else
    {
        use_modul_lager.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_modul_projekte(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_modul_projekte.zeile_ersaetzen(index, "1");
    }else
    {
        use_modul_projekte.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_modul_personal(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_modul_personal.zeile_ersaetzen(index, "1");
    }else
    {
        use_modul_personal.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_modul_matlist(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_modul_matlist.zeile_ersaetzen(index, "1");
    }else
    {
        use_modul_matlist.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_function_artfavsort(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_function_artfavsort.zeile_ersaetzen(index, "1");
    }else
    {
        use_function_artfavsort.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_function_projektdel(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_function_projektdel.zeile_ersaetzen(index, "1");
    }else
    {
        use_function_projektdel.zeile_ersaetzen(index, "0");
    }
}
void users::change_use_modul_bestellungen(uint index, bool isalowed)
{
    if(isalowed == true)
    {
        use_modul_bestellungen.zeile_ersaetzen(index, "1");
    }else
    {
        use_modul_bestellungen.zeile_ersaetzen(index, "0");
    }
}

void users::newuser()
{
    user_tz.zeile_anhaengen("Nutzer " + int_to_qstring(user_tz.zeilenanzahl()+1));
    pwd_tz.zeile_anhaengen("Nutzer " + int_to_qstring(user_tz.zeilenanzahl()));
    is_admin_tz.zeile_anhaengen("0");
    use_modul_artikel.zeile_anhaengen("0");
    use_modul_lieferanten.zeile_anhaengen("0");
    use_modul_lager.zeile_anhaengen("0");
    use_modul_projekte.zeile_anhaengen("0");
    id_tz.zeile_anhaengen("0");
    use_modul_personal.zeile_anhaengen("0");
    use_modul_matlist.zeile_anhaengen("0");
    use_function_artfavsort.zeile_anhaengen("0");
    use_function_projektdel.zeile_anhaengen("0");
    use_modul_bestellungen.zeile_anhaengen("0");
}

void users::removeuser(uint index)
{
    if(index <= user_tz.zeilenanzahl())
    {
        user_tz.zeile_loeschen(index);
        pwd_tz.zeile_loeschen(index);
        is_admin_tz.zeile_loeschen(index);
        use_modul_artikel.zeile_loeschen(index);
        use_modul_lieferanten.zeile_loeschen(index);
        use_modul_lager.zeile_loeschen(index);
        use_modul_projekte.zeile_loeschen(index);
        id_tz.zeile_loeschen(index);
        use_modul_personal.zeile_loeschen(index);
        use_modul_matlist.zeile_loeschen(index);
        use_function_artfavsort.zeile_loeschen(index);
        use_function_projektdel.zeile_loeschen(index);
        use_modul_bestellungen.zeile_loeschen(index);
    }
}

uint users::get_anz_admins()
{
    uint anz = 0;
    for(uint i=1; i<=is_admin_tz.zeilenanzahl() ;i++)
    {
        if(is_admin_tz.zeile(i) == "1")
        {
            anz++;
        }
    }
    return anz;
}
