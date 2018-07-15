#ifndef FORM_LAGER_H
#define FORM_LAGER_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "datum.h"

namespace Ui {
class Form_lager;
}

class Form_lager : public QWidget
{
    Q_OBJECT

public:
    explicit Form_lager(QWidget *parent = 0);
    ~Form_lager();

    void set_db(cbrainbatabase *new_db);
    void set_user(QString u);
    void show();

public slots:

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_lineEdit_suche_textChanged();

private:
    Ui::Form_lager *ui;

    cbrainbatabase *dbeigen;
    QSqlQueryModel *model;
    QString user;

    void update_table();
};

#endif // FORM_LAGER_H
