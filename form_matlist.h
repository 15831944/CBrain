#ifndef FORM_MATLIST_H
#define FORM_MATLIST_H

#include <QWidget>

#include "cbrainbatabase.h"
#include "_tabellennamen.h"

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

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::Form_matlist *ui;

    cbrainbatabase *dbeigen;

    QString user;
};

#endif // FORM_MATLIST_H
