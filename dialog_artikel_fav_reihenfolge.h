#ifndef DIALOG_ARTIKEL_FAV_REIHENFOLGE_H
#define DIALOG_ARTIKEL_FAV_REIHENFOLGE_H

#include <QDialog>

#include "cbrainbatabase.h"
#include "_tabname_artikel.h"
#include "text.h"

namespace Ui {
class Dialog_artikel_fav_reihenfolge;
}

class Dialog_artikel_fav_reihenfolge : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_artikel_fav_reihenfolge(QWidget *parent = 0);
    ~Dialog_artikel_fav_reihenfolge();

    void set_db(cbrainbatabase *new_db);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_up_clicked();
    void on_pushButton_down_clicked();

private:
    Ui::Dialog_artikel_fav_reihenfolge *ui;

    cbrainbatabase *dbeigen;

    void update_favorit();
};

#endif // DIALOG_ARTIKEL_FAV_REIHENFOLGE_H
