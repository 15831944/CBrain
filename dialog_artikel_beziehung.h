#ifndef DIALOG_ARTIKEL_BEZIEHUNG_H
#define DIALOG_ARTIKEL_BEZIEHUNG_H

#include <QDialog>

#include "cbrainbatabase.h"
#include "text_zeilenweise.h"
#include "text.h"
#include "dialog_dataselection.h"
#include "_tabellennamen.h"
#include "_tabname_artikel.h"
#include "umwandeln.h"

namespace Ui {
class Dialog_artikel_beziehung;
}

class Dialog_artikel_beziehung : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_artikel_beziehung(QWidget *parent = 0);
    ~Dialog_artikel_beziehung();

    void set_db(cbrainbatabase *new_db);
    inline void set_artikelid_eigen(QString id)
    {
        artikelid_eigen = id;
    }
    void set_data(text_zeilenweise data);
    void set_id_const(bool iskonst);

public slots:
    void slot_set_artikel(text_zeilenweise artikel_ids);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_cancel();

private slots:
    void on_pushButton_artikel_id_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();

private:
    Ui::Dialog_artikel_beziehung *ui;

    cbrainbatabase *dbeigen;
    QString artikelid_eigen;
    bool id_ist_const;

    void closeEvent(QCloseEvent *ce);
};

#endif // DIALOG_ARTIKEL_BEZIEHUNG_H
