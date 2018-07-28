#ifndef _TABELLENNAMEN_H
#define _TABELLENNAMEN_H

#define USER_NOBODY                 "nobody"
#define USER_NOBODY_ID              "0"

//--------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------
#define TABNAME_LIEFERANT           "table_lieferanten"
#define PARAM_LIEFERANT_ID          "id"                //Wert 0
#define PARAM_LIEFERANT_NAME        "Name"              //Wert 1
#define PARAM_LIEFERANT_ERSTELLER   "Ersteller"         //Wert 2
#define PARAM_LIEFERANT_DATERST     "Erfassung"         //Wert 3    //Datum der Erfassung/Erstellung
#define PARAM_LIEFERANT_BEARBEITER  "Bearbeiter"        //Wert 4
#define PARAM_LIEFERANT_DATBEARB    "Bearbeitung"       //Wert 5    //Datum der letzten Bearbeitung
#define PARAM_LIEFERANT_BLOCK       "blockiert"         //Wert 6    //Datensatz wurde vom eingetragenen Nutzer
                                                                    //zur Bearbeitung geöffnen
#define PARAM_LIEFERANT_KOMMENT     "Kommentar"         //Wert 7

//--------------------------------------------------------------------------------------------------------
#define TABNAME_PROJEKT             "table_projekte"
#define PARAM_PROJEKT_ID            "id"                //Wert 0
#define PARAM_PROJEKT_NAME          "Name"              //Wert 1
#define PARAM_PROJEKT_ERSTELLER     "Ersteller"         //Wert 2
#define PARAM_PROJEKT_DATERST       "Erfassung"         //Wert 3    //Datum der Erfassung/Erstellung
#define PARAM_PROJEKT_BEARBEITER    "Bearbeiter"        //Wert 4
#define PARAM_PROJEKT_DATBEARB      "Bearbeitung"       //Wert 5    //Datum der letzten Bearbeitung
#define PARAM_PROJEKT_BLOCK         "blockiert"         //Wert 6    //Datensatz wurde vom eingetragenen Nutzer
                                                                    //zur Bearbeitung geöffnen
#define PARAM_PROJEKT_KOMMENT       "Kommentar"         //Wert 7
//--------------------------------------------------------------------------------------------------------
#define TABNAME_LAGER               "table_lager"
#define PARAM_LAGER_ID              "id"                //Wert 0
#define PARAM_LAGER_VORGANG         "Vorgang"           //Wert 1    //Wareneingang | Warenausgang
#define PARAM_LAGER_ARTIKELID       "Artikel_id"        //Wert 2
#define PARAM_LAGER_MENGE           "Menge"             //Wert 3    //Eingelagerte bzw. ausgelagerte Menge
#define PARAM_LAGER_ERSTELLER       "Ersteller"         //Wert 4
#define PARAM_LAGER_DATERST         "Erfassung"         //Wert 5    //Datum der Erfassung/Erstellung
#define PARAM_LAGER_KOMMISSION      "Kommission"        //Wert 6    //Projektnummer
#define PARAM_LAGER_KOMMENT         "Kommentar"         //Wert 7

//--------------------------------------------------------------------------------------------------------
#define TABNAME_VORGANG             "table_vorgang"
#define PARAM_VORGANG_ID            "id"                //Wert 0
#define PARAM_VORGANG_KLARTEXT      "Vorgang"           //Wert 1
//----------------------------------
#define VORGANG_WARENEINGANG        "1"
#define VORGANG_WARENAUSGANG        "2"

//--------------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------------
#define TABNAME_RESERV              "table_reservierungen"
#define PARAM_RESERV_ID             "id"                //Wert 0
#define PARAM_RESERV_ARTIKELID      "ArtikelID"         //Wert 1
#define PARAM_RESERV_MENGE          "Menge"             //Wert 2
#define PARAM_RESERV_PROJID         "ProjektID"         //Wert 3







#endif // _TABELLENNAMEN_H
