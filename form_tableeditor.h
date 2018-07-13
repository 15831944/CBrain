#ifndef FORM_TABLEEDITOR_H
#define FORM_TABLEEDITOR_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "dialog_yes_no.h"
#include "dialog_text_input.h"
#include "dialog_tableparam.h"

namespace Ui {
class Form_tableeditor;
}

class Form_tableeditor : public QWidget
{
    Q_OBJECT

public:
    explicit Form_tableeditor(QWidget *parent = 0);
    ~Form_tableeditor();

    void set_db(cbrainbatabase *new_db);
    void clear();

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void slot_delete_table();
    void slot_delete_param();
    void slot_new_table(QString tablename);
    void slot_new_param(QString name, QString typ, QString additional, bool ispri, bool autoincrement);


private slots:
    void on_listWidget_tables_currentRowChanged();
    void on_listWidget_tablehead_currentRowChanged(int currentRow);
    void on_pushButton_table_new_clicked();
    void on_pushButton_table_del_clicked();
    void on_pushButton_param_new_clicked();
    void on_pushButton_param_del_clicked();
    void on_pushButton_param_edit_clicked();

private:
    Ui::Form_tableeditor *ui;

    cbrainbatabase *dbeigen;
    QSqlQueryModel *model;

};

#endif // FORM_TABLEEDITOR_H
