#ifndef FORM_ARTIKEL_H
#define FORM_ARTIKEL_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "_tabname_artikel.h"
#include "_tabname_personal.h"
#include "_tabname_lieferant.h"
#include "dialog_artikel.h"
#include "dialog_dataselection.h"
#include "text.h"
#include "datum.h"
#include "dialog_yes_no.h"

namespace Ui {
class Form_artikel;
}

class Form_artikel : public QWidget
{
    Q_OBJECT

public:
    explicit Form_artikel(QWidget *parent = 0);
    ~Form_artikel();

    void set_db(cbrainbatabase *new_db);
    void set_user(QString u);
    void show();

public slots:
    void slot_new(text_zeilenweise data);
    void slot_delete(text_zeilenweise ids);
    void slot_edit_dialog(text_zeilenweise ids);
    void slot_edit_dialog();
    void slot_edit_dialog_cancel();
    void slot_edit(text_zeilenweise data, QString id);

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButton_new_clicked();
    void on_lineEdit_suche_textChanged();
    void on_pushButton_del_clicked();
    void on_pushButton_edit_clicked();

private:
    Ui::Form_artikel *ui;

    cbrainbatabase *dbeigen;
    QSqlQueryModel *model;
    QString user;
    QString idbuffer;       //Speichert id
                            //von void slot_edit_dialog(text_zeilenweise ids);
                            //für void slot_edit_dialog();

    void update_table();
};

#endif // FORM_ARTIKEL_H
