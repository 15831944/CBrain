#ifndef FORM_LAGER_H
#define FORM_LAGER_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "_tabname_artikel.h"
#include "_tabname_personal.h"
#include "_tabname_projekt.h"
#include "_tabname_lager.h"
#include "_tabname_vorgang.h"
#include "_tabname_promat.h"
#include "_tabname_bestellung.h"
#include "_tabname_lieferant.h"
#include "datum.h"
#include "dialog_lager.h"
#include "umwandeln.h"
#include "dialog_text_input.h"
#include "dialog_lager_mengenkorrektur.h"

namespace Ui {
class Form_lager;
}

class Form_lager : public QWidget
{
    Q_OBJECT

public:
    explicit Form_lager(QWidget *parent = 0);
    ~Form_lager();

    void set_db(cbrainbatabase *new_db);
    void set_user(QString u);
    void show();

public slots:
    void slot_in(text_zeilenweise data);
    void slot_out(text_zeilenweise data);
    void slot_inagain(text_zeilenweise data);
    void slot_bestkor_aid(QString artikel_id);
    void slot_bestkor_menge(QString menge);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_lineEdit_suche_textChanged();
    void on_pushButton_in_clicked();
    void on_pushButton_out_clicked();
    void on_pushButton_inagain_clicked();
    void on_pushButton_korrektur_clicked();
    void on_comboBox_lieferanten_currentIndexChanged(int index);

private:
    Ui::Form_lager *ui;

    cbrainbatabase *dbeigen;
    QSqlQueryModel *model, *model_artikel;
    QString user;
    QString idbuffer;
    text_zeilenweise lieferanten_ids, lieferanten_namen;

    void update_table();
    void update_lieferanten();
};

#endif // FORM_LAGER_H
