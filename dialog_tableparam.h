#ifndef DIALOG_TABLEPARAM_H
#define DIALOG_TABLEPARAM_H

#include <QDialog>

namespace Ui {
class Dialog_tableparam;
}

class Dialog_tableparam : public QDialog
{
    Q_OBJECT

signals:
    void signal_send_dialog_data(QString name, QString typ, QString additional, bool ispri, bool autoincrement);

public:
    explicit Dialog_tableparam(QWidget *parent = 0);
    ~Dialog_tableparam();

    void set_windowTitle(QString title);
    void setup_mysql();

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();

    void on_comboBox_typ_currentIndexChanged(const QString &arg1);

private:
    Ui::Dialog_tableparam *ui;

    QString driver;
};

#endif // DIALOG_TABLEPARAM_H
