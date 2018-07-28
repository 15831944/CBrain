#ifndef DIALOG_DATASELECTION_H
#define DIALOG_DATASELECTION_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "umwandeln.h"

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
    void set_anz_returnwerte(int anz);

signals:
    void signal_send_selection(text_zeilenweise ids);

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_pushButton_all_clicked();
    void on_pushButton_non_clicked();
    void on_lineEdit_filter_textChanged();

private:
    Ui::Dialog_dataselection *ui;
    text_zeilenweise ids, filtered_ids;
    text_zeilenweise mydata, filtered_data;
    uint anzretwerte;

    void set_data(text_zeilenweise data);
    void update_listwidget();

};

#endif // DIALOG_DATASELECTION_H
