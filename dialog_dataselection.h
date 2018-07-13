#ifndef DIALOG_DATASELECTION_H
#define DIALOG_DATASELECTION_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"

namespace Ui {
class Dialog_dataselection;
}

class Dialog_dataselection : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_dataselection(QWidget *parent = 0);
    ~Dialog_dataselection();

    void set_data(text_zeilenweise data, text_zeilenweise id);

signals:
    void signal_send_selection(text_zeilenweise ids);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_pushButton_all_clicked();
    void on_pushButton_non_clicked();

private:
    Ui::Dialog_dataselection *ui;
    text_zeilenweise ids;

    void set_data(text_zeilenweise data);
};

#endif // DIALOG_DATASELECTION_H
