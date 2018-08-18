#ifndef _TODO_H
#define _TODO_H
/*


-->Konzept für Artikelpreis überlegen

-->user_A editiert ein Datenfeld
   user_B möchte auch editieren
   user_B bestätigt den dialog das user_A übergangen wird
   user_A kann nun die Änderung nicht mehr speichern
   wenn user_B nun jedoch dann nichts am Datansatz änder sondern mit cancel abbricht
   dann ist der Datensatz für die dannach folgende Änderung von user_A trotzdem noch blockiert
   wie den Knoten lösen?
   Wie user_A das speichern erlauben wenn user_B die Daten nicht verändert hat
   evtl Kopie des gesamten Datensatzes merken und vergleichen wenn datensatz nicht blockiert ist
   (block wird aufgehoben wenn user_B cancelt)
   dialog von user_A erwartet jedoch das die daten noch von ihm selbst blockiert sind
   ->evtl einfach nicht zulassen, dass user_B abbrechen kann (disable btn cancel)

-->Im-Export-Dialog

-->modul_lager
    ->Buschung von Waren-Ein- und Ausgang strornierbar machen (Falsche Buchungen korrigieren)
    ->Inventurfunktion zum korrigieren des Lagerbestandes
        >void Form_lager::slot_bestkor_menge(QString menge)
        >Dialog zum Ändern des Lagerbestandes erstellen

-->modul matliste
    ->Artikelpreis anzeigen lassen

-->Vorgänge in DB eintragen:
#define VORGANG_WIEDEREINLAGERUNG   "3" --> Wieder-Einlagerung
#define VORGANG_KORREKTUR           "4" --> Korrektur

























*/
#endif // _TODO_H
