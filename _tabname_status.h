#ifndef _TABNAME_STATUS_H
#define _TABNAME_STATUS_H

#define TABNAME_STATUS                "table_status"

#define PARAM_STATUS_ID               "id"                //Wert 0
#define PARAM_STATUS_STATUS           "Status"            //Wert 1


/*
Werte:
01  erfasst
02  Klärungsbedarf
03  zur Bestellung freigegeben

*/
#define STATUS_ERFASST   "1"
#define STATUS_UNKLAR    "2"
#define STATUS_BESTELLEN "3"

#endif // _TABNAME_STATUS_H
