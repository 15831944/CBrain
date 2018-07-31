#ifndef FORM_MATLIST_H
#define FORM_MATLIST_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "_tabname_projekt.h"
#include "_tabname_promatpos.h"
#include "_tabname_promatposlist.h"
#include "dialog_dataselection.h"
#include "dialog_promatpos.h"

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

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButton_projektauswahl_clicked();
    void on_lineEdit_projekt_id_textChanged(const QString &arg1);
    void on_pushButton_pos_new_clicked();

private:
    Ui::Form_matlist *ui;

    cbrainbatabase *dbeigen;

    QString user;

    void create_table_promatposlist();  //Tabelle die die Materialpositionen erfasst
    void create_table_promatpos(QString bez, QString menge); //Tabelle die den Inhalt einer Materialposition erfasst
    void update_listwidget_matpos();
};

#endif // FORM_MATLIST_H
