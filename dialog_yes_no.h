#ifndef DIALOG_YES_NO_H
#define DIALOG_YES_NO_H

#include <QDialog>

namespace Ui {
class Dialog_yes_no;
}

class Dialog_yes_no : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_yes_no(QWidget *parent = 0);
    ~Dialog_yes_no();
    void setup(QString msg);

signals:
    void signal_yes();
    void signal_no();

private slots:
    void on_pushButton_yes_clicked();

    void on_pushButton_no_clicked();

private:
    Ui::Dialog_yes_no *ui;
};

#endif // DIALOG_YES_NO_H
