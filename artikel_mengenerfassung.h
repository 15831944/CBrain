#ifndef ARTIKEL_MENGENERFASSUNG_H
#define ARTIKEL_MENGENERFASSUNG_H

#include "text_zeilenweise.h"
#include "_tabellennamen.h"
#include "text.h"
#include "_tabname_status.h"
#include "umwandeln.h"

class artikel_mengenerfassung
{
public:
    artikel_mengenerfassung();

    void add_matpos(text_zeilenweise artikel_ids,   \
                    text_zeilenweise mengen,        \
                    text_zeilenweise status_ids,   \
                    text_zeilenweise beziehungen    );

    text_zeilenweise get_artikel_ids();
    text_zeilenweise get_mengen_gesamt();
    text_zeilenweise get_mengen_erfasst();
    text_zeilenweise get_mengen_unklar();
    text_zeilenweise get_mengen_bestellen();

private:
    text_zeilenweise artikel_ids;
    text_zeilenweise mengen_erfasst;
    text_zeilenweise mengen_unklar;
    text_zeilenweise mengen_bestellen;

    void add_artikel(QString id, QString menge, QString status);
    uint artikel_id_bereits_erfasst(QString id);
};

#endif // ARTIKEL_MENGENERFASSUNG_H
