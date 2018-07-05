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

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::Form_tableeditor *ui;

    cbrainbatabase *db;
};

#endif // FORM_TABLEEDITOR_H
