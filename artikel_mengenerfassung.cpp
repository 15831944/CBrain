#include "artikel_mengenerfassung.h"

artikel_mengenerfassung::artikel_mengenerfassung()
{

}

void artikel_mengenerfassung::add_matpos(text_zeilenweise artikel_ids, \
                                         text_zeilenweise mengen, \
                                         text_zeilenweise status_ids, \
                                         text_zeilenweise beziehungen)
{
    for(uint i=1; i<=artikel_ids.zeilenanzahl() ;i++)
    {
        QString akt_artikel_id  = artikel_ids.zeile(i);
        QString akt_menge       = mengen.zeile(i);
        QString akt_status      = status_ids.zeile(i);

        if(!akt_artikel_id.isEmpty())
        {
            add_artikel(akt_artikel_id, akt_menge, akt_status);
        }

        QString akt_beziehungen = beziehungen.zeile(i);
        akt_beziehungen.replace(NEW_LINE_BR, "\n");
        text_zeilenweise akt_beziehungen_tz;
        akt_beziehungen_tz.set_text(akt_beziehungen);
        for(uint ii=1; ii<=akt_beziehungen_tz.zeilenanzahl() ;ii++)
        {
            QString zeile = akt_beziehungen_tz.zeile(i);
            akt_artikel_id  = text_links(zeile, " ||| ");
            double menge_bezugsartikel = akt_menge.toDouble();
            akt_menge       = text_rechts(zeile, " ||| ");
            double akt_menge_double = menge_bezugsartikel * akt_menge.toDouble();
            akt_menge = double_to_qstring(akt_menge_double);

            if(!akt_artikel_id.isEmpty())
            {
                add_artikel(akt_artikel_id, akt_menge, akt_status);
            }
        }
    }
}

void artikel_mengenerfassung::add_artikel(QString id, QString menge, QString status)
{
    uint index = artikel_id_bereits_erfasst(id);
    if(index > 0)
    {
        if(status == STATUS_ERFASST)
        {
           QString erfasste_menge = mengen_erfasst.zeile(index);
           double neue_menge = erfasste_menge.toDouble() + menge.toDouble();
           mengen_erfasst.zeile_ersaetzen(index, double_to_qstring(neue_menge));
        }else if(status == STATUS_UNKLAR)
        {
            QString erfasste_menge = mengen_unklar.zeile(index);
            double neue_menge = erfasste_menge.toDouble() + menge.toDouble();
            mengen_unklar.zeile_ersaetzen(index, double_to_qstring(neue_menge));
        }else if(status == STATUS_BESTELLEN)
        {
            QString erfasste_menge = mengen_bestellen.zeile(index);
            double neue_menge = erfasste_menge.toDouble() + menge.toDouble();
            mengen_bestellen.zeile_ersaetzen(index, double_to_qstring(neue_menge));
        }
    }else
    {
        artikel_ids.zeile_anhaengen(id);
        if(status == STATUS_ERFASST)
        {
            mengen_erfasst.zeile_anhaengen(menge);
            mengen_unklar.zeile_anhaengen("0");
            mengen_bestellen.zeile_anhaengen("0");
        }else if(status == STATUS_UNKLAR)
        {
            mengen_erfasst.zeile_anhaengen("0");
            mengen_unklar.zeile_anhaengen(menge);
            mengen_bestellen.zeile_anhaengen("0");
        }else if(status == STATUS_BESTELLEN)
        {
            mengen_erfasst.zeile_anhaengen("0");
            mengen_unklar.zeile_anhaengen("0");
            mengen_bestellen.zeile_anhaengen(menge);
        }
    }
}

uint artikel_mengenerfassung::artikel_id_bereits_erfasst(QString id)
{
    //Wenn die ID bereits erfasst ist gibt die Funktion den index im tz zurück
    //andernfalls 0 == false
    uint erfasst = 0;
    for(uint i=1; i<=artikel_ids.zeilenanzahl() ;i++)
    {
        if(artikel_ids.zeile(i) == id)
        {
            erfasst = i;
            break;
        }
    }
    return erfasst;
}

text_zeilenweise artikel_mengenerfassung::get_artikel_ids()
{
    return artikel_ids;
}

text_zeilenweise artikel_mengenerfassung::get_mengen_gesamt()
{
    text_zeilenweise gesamtmengen;
    for(uint i=1; i<=mengen_erfasst.zeilenanzahl() ;i++)
    {
        double erfasst      = mengen_erfasst.zeile(i).toDouble();
        double unklar       = mengen_unklar.zeile(i).toDouble();
        double bestellen    = mengen_bestellen.zeile(i).toDouble();
        double gesamt = erfasst + unklar + bestellen;
        gesamtmengen.zeile_anhaengen(double_to_qstring(gesamt));
    }
    return gesamtmengen;
}

text_zeilenweise artikel_mengenerfassung::get_mengen_erfasst()
{
    return mengen_erfasst;
}
text_zeilenweise artikel_mengenerfassung::get_mengen_unklar()
{
    return mengen_unklar;
}
text_zeilenweise artikel_mengenerfassung::get_mengen_bestellen()
{
    return mengen_bestellen;
}








