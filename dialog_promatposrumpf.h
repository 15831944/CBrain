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

protected:
    void resizeEvent(QResizeEvent *event);

private:
    Ui::Dialog_promatposrumpf *ui;

    QString tabname;
    QString current_userid;
    cbrainbatabase *dbeigen;
    QSqlQueryModel *model;

    void update_table();
    void update_favorit();
};

#endif // DIALOG_PROMATPOSRUMPF_H
