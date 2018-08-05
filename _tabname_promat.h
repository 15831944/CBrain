#ifndef _TABNAME_PROMAT_H
#define _TABNAME_PROMAT_H

/*
    Erfasst die Materialmengen eines Projektes

    Wird in "form_projekt" angelegt beim Anlagen eines neuen Projektes
*/

#define TABNAME_PROMAT                "promat_"           //Nach dem _ folgt die Projekt-ID

#define PARAM_PROMAT_ID               "id"                //Wert 0
#define PARAM_PROMAT_ARTIKEL_ID       "Artikel_ID"        //Wert 1
#define PARAM_PROMAT_ME_VERARBEITET   "verarbeitet"       //Wert 2
#define PARAM_PROMAT_ME_GESAMTBEDARF  "Gesamtbedarf"      //Wert 3 gesamte Megen
#define PARAM_PROMAT_ME_ERFASST       "erfasst"           //Wert 4 davon mit Status erfasst
#define PARAM_PROMAT_ME_UNKLAR        "unklar"            //Wert 5 davon mit Status unklar
#define PARAM_PROMAT_ME_ZURBEST       "bestellen"         //Wert 6 davon mit Status bestellen



#endif // _TABNAME_PROMAT_H
