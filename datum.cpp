#include "datum.h"

datum::datum()
{

}

QString datum::get_today_y_m_d()
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

    return msg;
}
