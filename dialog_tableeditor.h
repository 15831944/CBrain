#ifndef DIALOG_TABLEEDITOR_H
#define DIALOG_TABLEEDITOR_H

#include <QDialog>

#include "cbrainbatabase.h"
#include "text_zeilenweise.h"

namespace Ui {
class Dialog_tableeditor;
}

class Dialog_tableeditor : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_tableeditor(QWidget *parent = 0);
    ~Dialog_tableeditor();

    void set_db(cbrainbatabase *new_db);
    void set_table(QString t);

signals:
    void signal_ok();

private slots:
    void on_pushButton_ok_clicked();
    void on_listWidget_zeilen_currentRowChanged();
    void on_pushButton_new_clicked();
    void on_pushButton_del_clicked();
    void on_comboBox_param_currentIndexChanged(int index);
    void on_comboBox_param2_currentIndexChanged(int index);
    void on_comboBox_param3_currentIndexChanged(int index);
    void on_comboBox_param4_currentIndexChanged(int index);
    void on_comboBox_param5_currentIndexChanged(int index);
    void on_comboBox_param6_currentIndexChanged(int index);
    void on_comboBox_param7_currentIndexChanged(int index);
    void on_comboBox_param8_currentIndexChanged(int index);
    void on_comboBox_param9_currentIndexChanged(int index);
    void on_lineEdit_value_editingFinished();
    void on_lineEdit_value2_editingFinished();
    void on_lineEdit_value3_editingFinished();
    void on_lineEdit_value4_editingFinished();
    void on_lineEdit_value5_editingFinished();
    void on_lineEdit_value6_editingFinished();
    void on_lineEdit_value7_editingFinished();
    void on_lineEdit_value8_editingFinished();
    void on_lineEdit_value9_editingFinished();

private:
    Ui::Dialog_tableeditor *ui;

    cbrainbatabase *dbeigen;
    QString table;
    text_zeilenweise zeilen_id;

    void update_gui();
    void combobox_default_setup();
};

#endif // DIALOG_TABLEEDITOR_H
