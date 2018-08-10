#include "datum.h"

datum::datum()
{

}

QString datum::get_today_y_m_d()
{
    QDate today = QDate::currentDate();
    return get_datum_y_m_d(&today);
}

QString datum::get_today_y_m_d_h_m_s()
{
    int y,m,d;
    QDate today = QDate::currentDate();
    today.getDate(&y, &m, &d);

    QString msg;
    msg += int_to_qstring(y);
    msg += "-";
    msg += int_to_qstring(m);
    msg += "-";
    msg += int_to_qstring(d);

    msg += " ";

    QTime jetzt = QTime::currentTime();
    int stunde, minute, sekunde;
    stunde = jetzt.hour();
    minute = jetzt.minute();
    sekunde = jetzt.second();

    msg += int_to_qstring(stunde);
    msg += ":";
    msg += int_to_qstring(minute);
    msg += ":";
    msg += int_to_qstring(sekunde);

    return msg;
}

QString datum::get_datum_y_m_d(QDate *da)
{
    int y,m,d;
    da->getDate(&y, &m, &d);

    QString msg;
    msg += int_to_qstring(y);
    msg += "-";
    msg += int_to_qstring(m);
    msg += "-";
    msg += int_to_qstring(d);

    return msg;
}
