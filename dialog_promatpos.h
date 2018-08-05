#ifndef DIALOG_PROMATPOS_H
#define DIALOG_PROMATPOS_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "umwandeln.h"
#include "_tabellennamen.h"

namespace Ui {
class Dialog_promatpos;
}

class Dialog_promatpos : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_promatpos(QWidget *parent = 0);
    ~Dialog_promatpos();

    void set_data(text_zeilenweise daten, QString id);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_send_data(text_zeilenweise data, QString id);
    void signal_cancel();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_promatpos *ui;

    QString current_id;
    void closeEvent(QCloseEvent *ce);
};

#endif // DIALOG_PROMATPOS_H
