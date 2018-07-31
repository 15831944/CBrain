#ifndef _TABNAME_PROMATPOS_H
#define _TABNAME_PROMATPOS_H

/*
    Erfasst den Mertialbedarf für eine Positionen des Projektes
    Eine Tabelle pro Projektposition
*/

#define TABNAME_PROMATPOS             "promatpos_"        //Nach dem "_" folgt die Projekt-ID
                                                            //dann folgt ein weiterer "_"
                                                            //dann folgt die Positions-ID
#define PARAM_PROMATPOS_ID           "id"                //Wert 0
#define PARAM_PROMATPOS_ARTIKEL_ID   "Artikel_id"        //Wert 1
#define PARAM_PROMATPOS_MENGE        "Menge"             //Wert 2
#define PARAM_PROMATPOS_STATUS_ID    "Status"            //Wert 3
#define PARAM_PROMATPOS_ERSTELLER    "Ersteller"         //Wert 4
#define PARAM_PROMATPOS_BEARBEITER   "Bearbeiter"        //Wert 5
#define PARAM_PROMATPOS_BLOCK        "blockiert"         //Wert 6



#endif // _TABNAME_PROMATPOS_H
