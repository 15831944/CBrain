#ifndef DIALOG_LAGER_H
#define DIALOG_LAGER_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "_tabname_artikel.h"
#include "_tabname_lieferant.h"
#include "_tabname_projekt.h"
#include "_tabname_lieferant.h"
#include "text.h"
#include "dialog_printbox.h"
#include "dialog_yes_no.h"

namespace Ui {
class Dialog_lager;
}

class Dialog_lager : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_lager(QWidget *parent = 0);
    ~Dialog_lager();

    void set_db(cbrainbatabase *new_db);
    void setup();

    inline void set_vorgang(QString v)
    {
        vorgang = v;
    }
    void set_kommission_enabled(bool isit);
    void set_lieferschein_enabled(bool isit);
    void set_showinbestellung_enabled(bool isit);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_cancel();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_lineEdit_artikelfilter_textChanged();
    void on_lineEdit_komfilter_textChanged(const QString &arg1);
    void on_pushButton_print_clicked();
    void on_lineEdit_lieferantenfilter_textChanged(const QString &arg1);
    void slot_oksameagain();
    void on_comboBox_artikel_currentIndexChanged(int index);
    void on_comboBox_kom_currentIndexChanged(int index);

    void on_comboBox_lieferanten_currentIndexChanged(int index);

private:
    Ui::Dialog_lager *ui;

    cbrainbatabase *dbeigen;
    QString current_id;

    text_zeilenweise artikel_id,        artikel_id_gefiltert;
    text_zeilenweise artikel_nr,        artikel_nr_gefiltert;
    text_zeilenweise artikel_bez,       artikel_bez_gefiltert;
    text_zeilenweise artikel_lie_id,    artikel_lie_id_gefiltert;//Lieferant

    text_zeilenweise lie_id,            lie_id_gefiltert;
    text_zeilenweise lie_name,          lie_name_gefiltert;

    text_zeilenweise projekt_id,        projekt_id_gefiltert;
    text_zeilenweise projekt_name,      projekt_name_gefiltert;

    text_zeilenweise lastokmsg;

    //text_zeilenweise lieferanten;

    void update_artikel();
    QString printmsg;
    QString vorgang;

    void dlg_to_printmsg();
    void closeEvent(QCloseEvent *ce);
};

#endif // DIALOG_LAGER_H
