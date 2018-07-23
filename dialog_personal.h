#ifndef DIALOG_PERSONAL_H
#define DIALOG_PERSONAL_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "cbrainbatabase.h"
#include "_tabellennamen.h"

namespace Ui {
class Dialog_personal;
}

class Dialog_personal : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_personal(QWidget *parent = 0);
    ~Dialog_personal();

    void set_data(text_zeilenweise daten, QString id);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_send_data(text_zeilenweise data, QString id);
    void signal_cancel();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_personal *ui;

    QString current_id;

    void clear();
};

#endif // DIALOG_PERSONAL_H
