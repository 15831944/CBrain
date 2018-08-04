#ifndef DIALOG_PROMATPOSRUMPF_H
#define DIALOG_PROMATPOSRUMPF_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "_tabname_artikel.h"
#include "_tabname_status.h"
#include "_tabname_promatpos.h"
#include "_tabname_personal.h"
#include "dialog_artikel_verwenden.h"
#include "text.h"
#include "dialog_dataselection.h"
#include "dialog_yes_no.h"

namespace Ui {
class Dialog_promatposrumpf;
}

class Dialog_promatposrumpf : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_promatposrumpf(QWidget *parent = 0);
    ~Dialog_promatposrumpf();

    void set_tabname(QString name);
    void set_userid(QString id);
    void set_db(cbrainbatabase *new_db);
    void show();

public slots:
    void slot_new_data(text_zeilenweise data);
    void slot_edit_data(text_zeilenweise data);
    void slot_delete(text_zeilenweise ids);
    void slot_new_artikel(text_zeilenweise artikel_ids);//muss als tz wegen Dialog_dataselection
    void slot_edit_dialog(text_zeilenweise ids);
    void slot_edit_dialog();
    void slot_edit_dialog_cancel();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButton_usefavourite_clicked();
    void on_pushButton_delete_clicked();
    void on_lineEdit_filter_textChanged();
    void on_pushButton_new_clicked();
    void on_pushButton_edit_clicked();


private:
    Ui::Dialog_promatposrumpf *ui;

    QString tabname;
    QString current_userid;
    cbrainbatabase *dbeigen;
    QSqlQueryModel *model;

    text_zeilenweise fav_id_tz, fav_nr_tz, fav_bez_tz;

    QString idbuffer;       //Speichert id der eigenen tabelle


    void update_table();
    void update_favorit();
    bool artikel_bereits_erfasst(QString artikel_id);
    QString get_own_id_from_artikel_id(QString artikel_id);
    void bearbeiten_dialog(QString artikel_id);
};

#endif // DIALOG_PROMATPOSRUMPF_H
