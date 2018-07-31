#ifndef _TABNAME_PROMATPOSLIST_H
#define _TABNAME_PROMATPOSLIST_H

/*
    Erfasst die Mertial-Positionen eines Projektes
    Eine Tabelle pro Projekt
*/

#define TABNAME_PROMATPOSLIST             "promatpos_"        //Nach dem _ folgt die Projekt-ID

#define PARAM_PROMATPOSLIST_ID            "id"                //Wert 0
#define PARAM_PROMATPOSLIST_BEZ           "Bezeichnung"       //Wert 1
#define PARAM_PROMATPOSLIST_MENGE         "Menge"             //Wert 2  Wie oft kommt die Position vor
#define PARAM_PROMATPOSLIST_ERSTELLER     "Ersteller"         //Wert 3
#define PARAM_PROMATPOSLIST_BEARBEITER    "Bearbeiter"        //Wert 4
#define PARAM_PROMATPOSLIST_BLOCK         "blockiert"         //Wert 5    //Datensatz wurde vom eingetragenen Nutzer
                                                                          //zur Bearbeitung geöffnen



#endif // _TABNAME_PROMATPOSLIST_H
