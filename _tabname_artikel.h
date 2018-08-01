#ifndef _TABNAME_ARTIKEL_H
#define _TABNAME_ARTIKEL_H


#define TABNAME_ARTIKEL             "table_artikel"
#define PARAM_ARTIKEL_ID            "id"                //Wert 0
#define PARAM_ARTIKEL_NR            "Artikelnummer"     //Wert 1
#define PARAM_ARTIKEL_BEZ           "Bezeichnung"       //Wert 2
#define PARAM_ARTIKEL_LIEFERANT     "Lieferant"         //Wert 3
#define PARAM_ARTIKEL_LAGERSTAND    "Lagerbestand"      //Wert 4
#define PARAM_ARTIKEL_ERSTELLER     "Ersteller"         //Wert 5
#define PARAM_ARTIKEL_DATERST       "Erfassung"         //Wert 6    //Datum der Erfassung/Erstellung
#define PARAM_ARTIKEL_BEARBEITER    "Bearbeiter"        //Wert 7
#define PARAM_ARTIKEL_DATBEARB      "Bearbeitung"       //Wert 8    //Datum der letzten Bearbeitung
#define PARAM_ARTIKEL_BLOCK         "blockiert"         //Wert 9    //Datensatz wurde vom eingetragenen Nutzer
                                                                    //zur Bearbeitung geöffnen
#define PARAM_ARTIKEL_LAGERORT      "Lagerort"          //Wert 10
#define PARAM_ARTIKEL_VE            "VE"                //Wert 11   //Verpackungseinheit = Menge pro Packung
#define PARAM_ARTIKEL_KOMENT        "Kommentar"         //Wert 12
#define PARAM_ARTIKEL_BEZIEHUNG     "Beziehung"         //Wert 13



/*
Aufbau Spalte Beziehung:
id          == Artikelid des Artikels der in beziehung gesetzt wird
default     -> 0 == wird vorgeschlagen als uncheched
            -> 1 == wird vorgeschlagen als cheched
menge       == Wie oft wird der Artikel gebraucht pro Parent-Artikel

id ||| default ||| menge \n
id ||| default ||| menge \n
id ||| default ||| menge
*/






#endif // _TABNAME_ARTIKEL_H
