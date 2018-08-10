#ifndef DIALOG_DATUM_INPUT_H
#define DIALOG_DATUM_INPUT_H

#include <QDialog>

namespace Ui {
class Dialog_datum_input;
}

class Dialog_datum_input : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_datum_input(QWidget *parent = 0);
    ~Dialog_datum_input();

    void set_date(QDate datum);

signals:
    void signal_cancel();
    void signal_send_date(QDate *da);

private slots:
    void on_pushButton_cancel_clicked();
    void on_dateEdit_userDateChanged(const QDate &date);
    void on_calendarWidget_clicked(const QDate &date);
    void on_pushButton_ok_clicked();

private:
    Ui::Dialog_datum_input *ui;
};

#endif // DIALOG_DATUM_INPUT_H
