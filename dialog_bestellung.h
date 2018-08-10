#ifndef DIALOG_BESTELLUNG_H
#define DIALOG_BESTELLUNG_H

#include <QDialog>
#include <QDate>

#include "text_zeilenweise.h"
#include "umwandeln.h"

namespace Ui {
class Dialog_bestellung;
}

class Dialog_bestellung : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_bestellung(QWidget *parent = 0);
    ~Dialog_bestellung();

    void set_data(text_zeilenweise data);

signals:
    void signal_cancel();
    void signal_send_data(text_zeilenweise data);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_calendarWidget_LT_clicked();

private:
    Ui::Dialog_bestellung *ui;
};

#endif // DIALOG_BESTELLUNG_H
