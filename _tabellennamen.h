#ifndef _TABELLENNAMEN_H
#define _TABELLENNAMEN_H

#define USER_NOBODY                 "nobody"

#define TABNAME_ARTIKEL             "Artikel"
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

#define TABNAME_LIEFERANTEN         "Lieferanten"
#define PARAM_LIEFERANT_ID          "id"                //Wert 0
#define PARAM_LIEFERANT_NAME        "Name"              //Wert 1
#define PARAM_LIEFERANT_ERSTELLER   "Ersteller"         //Wert 3
#define PARAM_LIEFERANT_DATERST     "Erfassung"         //Wert 4    //Datum der Erfassung/Erstellung
#define PARAM_LIEFERANT_BEARBEITER  "Bearbeiter"        //Wert 5
#define PARAM_LIEFERANT_DATBEARB    "Bearbeitung"       //Wert 6    //Datum der letzten Bearbeitung
#define PARAM_LIEFERANT_BLOCK       "blockiert"         //Wert 7    //Datensatz wurde vom eingetragenen Nutzer
                                                                    //zur Bearbeitung geöffnen

#define TABNAME_RESERV              "Reservierungen"
#define PARAM_RESERV_ID             "id"                //Wert 0
#define PARAM_RESERV_ARTIKELID      "ArtikelID"         //Wert 1
#define PARAM_RESERV_MENGE          "Menge"             //Wert 2
#define PARAM_RESERV_PROJID         "ProjektID"         //Wert 3







#endif // _TABELLENNAMEN_H
