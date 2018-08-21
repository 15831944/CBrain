#include"umwandeln.h"


std::string int_to_string(int zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return returnstring;
}

QString int_to_qstring(int zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return QString::fromStdString(returnstring);
}

std::string float_to_string(float zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return returnstring;
}

QString float_to_qstring(float zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return QString::fromStdString(returnstring);
}

std::string double_to_string(double zahl)
{
    std::string returnstring;
    std::stringstream ss;
    ss << zahl;
    returnstring = ss.str();
    return returnstring;
}

QString double_to_qstring(double zahl)
{
    QString msg = QString::number(zahl, 'f', 4);//4 sorgt für 4 Nachkommastellen
    return msg;
}

QString double_to_qstring_euro(double zahl)
{
    QString msg = QString::number(zahl, 'f', 2);//2 sorgt für 2 Nachkommastellen
    msg.replace(".",",");
    msg += "€";
    return msg;
}
