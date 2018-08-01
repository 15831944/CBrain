#ifndef FORM_MATLIST_H
#define FORM_MATLIST_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "_tabname_projekt.h"
#include "_tabname_promatpos.h"
#include "_tabname_promatposlist.h"
#include "_tabname_personal.h"
#include "dialog_dataselection.h"
#include "dialog_promatpos.h"
#include "dialog_promatposrumpf.h"
#include "text.h"
#include "dialog_yes_no.h"

namespace Ui {
class Form_matlist;
}

class Form_matlist : public QWidget
{
    Q_OBJECT

public:
    explicit Form_matlist(QWidget *parent = 0);
    ~Form_matlist();

    void set_db(cbrainbatabase *new_db);
    void set_user(QString u);

public slots:
    void slot_set_project(text_zeilenweise p);
    void slot_new_matpos(text_zeilenweise data);
    void slot_edit_matposlist(text_zeilenweise data, QString id);
    void slot_edit_matposlist();
    void slot_edit_matposlist_with_block();
    void slot_edit_matposlist_unblock();
    void slot_delete_matpos();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButton_projektauswahl_clicked();
    void on_lineEdit_projekt_id_textChanged(const QString &arg1);
    void on_pushButton_pos_new_clicked();
    void on_pushButton_pos_edit_clicked();
    void on_pushButton_pos_delete_clicked();
    void on_pushButton_check_all_pos_clicked();
    void on_pushButton_check_activ_pos_clicked();

    void on_pushButton_pos_edit_rumpf_clicked();

private:
    Ui::Form_matlist *ui;
    cbrainbatabase *dbeigen;
    QString user;
    text_zeilenweise promatposlist_current_data;
    QString promatposlist_current_id;

    void create_table_promatposlist();  //Tabelle die die Materialpositionen erfasst
    void create_table_promatpos(QString bez, QString menge); //Tabelle die den Inhalt einer Materialposition erfasst
    void update_listwidget_matpos();
};

#endif // FORM_MATLIST_H
