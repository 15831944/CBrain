#ifndef DATUM_H
#define DATUM_H

#include <QString>
#include <QDate>
#include <QTime>
#include "umwandeln.h"

class datum
{
public:
    datum();

    QString get_today_y_m_d();
    QString get_today_y_m_d_h_m_s();

};

#endif // DATUM_H
