#ifndef DIALOG_LIEFERANTEN_H
#define DIALOG_LIEFERANTEN_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "cbrainbatabase.h"
#include "_tabellennamen.h"

namespace Ui {
class Dialog_lieferanten;
}

class Dialog_lieferanten : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_lieferanten(QWidget *parent = 0);
    ~Dialog_lieferanten();

    void set_data(text_zeilenweise daten, QString id);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_send_data(text_zeilenweise data, QString id);
    void signal_cancel();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_lieferanten *ui;

    QString current_id;

    void clear();
    void closeEvent(QCloseEvent *ce);
};

#endif // DIALOG_LIEFERANTEN_H
