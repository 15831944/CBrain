#ifndef DIALOG_LAGER_MENGENKORREKTUR_H
#define DIALOG_LAGER_MENGENKORREKTUR_H

#include <QDialog>

#include "cbrainbatabase.h"
#include "_tabname_artikel.h"
#include "umwandeln.h"

namespace Ui {
class Dialog_lager_mengenkorrektur;
}

class Dialog_lager_mengenkorrektur : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_lager_mengenkorrektur(QWidget *parent = 0);
    ~Dialog_lager_mengenkorrektur();

    void set_db(cbrainbatabase *new_db);
    void set_aid(QString artikelid);

signals:
    void signal_send_menge(QString me);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();

private:
    Ui::Dialog_lager_mengenkorrektur *ui;

    cbrainbatabase *dbeigen;
};

#endif // DIALOG_LAGER_MENGENKORREKTUR_H
