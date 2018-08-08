#ifndef FORM_BESTELLUNG_H
#define FORM_BESTELLUNG_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabname_artikel.h"
#include "_tabname_lieferant.h"

namespace Ui {
class Form_bestellung;
}

class Form_bestellung : public QWidget
{
    Q_OBJECT

public:
    explicit Form_bestellung(QWidget *parent = 0);
    ~Form_bestellung();

    void set_db(cbrainbatabase *new_db);
    void set_user(QString u);
    void show();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_lineEdit_suche_textChanged(const QString &arg1);

private:
    Ui::Form_bestellung *ui;

    cbrainbatabase *dbeigen;
    QSqlQueryModel *model;
    QString user;

    void update_table();
};

#endif // FORM_BESTELLUNG_H
