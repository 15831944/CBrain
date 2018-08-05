#ifndef DIALOG_ARTIKEL_VERWENDEN_H
#define DIALOG_ARTIKEL_VERWENDEN_H

#include <QDialog>

#include "cbrainbatabase.h"
#include "text_zeilenweise.h"
#include "_tabellennamen.h"
#include "_tabname_artikel.h"
#include "_tabname_status.h"
#include "text.h"

namespace Ui {
class Dialog_artikel_verwenden;
}

class Dialog_artikel_verwenden : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_artikel_verwenden(QWidget *parent = 0);
    ~Dialog_artikel_verwenden();

    void set_db(cbrainbatabase *new_db);
    void set_artikel_id(QString id);
    void set_data(text_zeilenweise data);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_cancel();

private slots:
    void on_pushButton_cancel_clicked();
    void on_lineEdit_id_textChanged(const QString &arg1);
    void on_pushButton_ok_clicked();

private:
    Ui::Dialog_artikel_verwenden *ui;

    cbrainbatabase *dbeigen;
    text_zeilenweise status_tz, status_id_tz;
    text_zeilenweise bezi_id_tz, bezi_nr_tz, bezi_bez_tz, bezi_default_tz, bezi_menge_tz;

};

#endif // DIALOG_ARTIKEL_VERWENDEN_H
