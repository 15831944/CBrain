#ifndef FORM_TABLEEDITOR_H
#define FORM_TABLEEDITOR_H

#include <QWidget>
#include "cbrainbatabase.h"

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

    inline void show()
    {
        setVisible(true);
        //mydb = QSqlDatabase::addDatabase(db->get_driver(), "noname");
    }
    inline void hide()
    {
        setVisible(false);
        //mydb = QSqlDatabase();
        //mydb.removeDatabase("noname");
    }

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_listWidget_tables_currentRowChanged();

    void on_listWidget_tablehead_currentRowChanged(int currentRow);

private:
    Ui::Form_tableeditor *ui;

    cbrainbatabase *dbeigen;
    //QSqlDatabase mydb;
    QSqlQueryModel *model;
};

#endif // FORM_TABLEEDITOR_H
