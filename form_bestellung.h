#ifndef FORM_BESTELLUNG_H
#define FORM_BESTELLUNG_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabname_artikel.h"
#include "_tabname_lieferant.h"
#include "_tabname_promat.h"
#include "text.h"

namespace Ui {
class Form_bestellung;
}

class Form_bestellung : public QWidget
{
    Q_OBJECT

public:
    explicit Form_bestellung(QWidget *parent = 0);
    ~Form_bestellung();

    void set_db(cbrainbatabase *new_db);
    void set_user(QString u);
    void show();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_lineEdit_suche_textChanged(const QString &arg1);

    void on_lineEdit_suche_vorschlag_textChanged(const QString &arg1);

    void on_comboBox_vorschlag_lieferant_currentIndexChanged(const QString &arg1);

private:
    Ui::Form_bestellung *ui;

    cbrainbatabase *dbeigen;
    QSqlQueryModel *model;
    QString user;
    //tzs zum Speichern der Bestellvorschläge:
    text_zeilenweise besvor_artikel_id,         besvor_artikel_id_gefiltert;
    text_zeilenweise besvor_artikel_nr,         besvor_artikel_nr_gefiltert;
    text_zeilenweise besvor_artikel_bez,        besvor_artikel_bez_gefiltert;
    text_zeilenweise besvor_menge,              besvor_menge_gefiltert;
    text_zeilenweise besvor_artikel_lieferant,  besvor_artikel_lieferant_gefiltert;

    void update_table();
    void update_bestellvor();
    void update_bestellvor_tz();
    void besvor_filtern();
    void update_bestellvor_listwidget();
    void update_bestellvor_listwidget_items();
};

#endif // FORM_BESTELLUNG_H
