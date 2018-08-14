#ifndef DIALOG_TEXT_INPUT_H
#define DIALOG_TEXT_INPUT_H

#include <QDialog>

namespace Ui {
class Dialog_text_input;
}

class Dialog_text_input : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_text_input(QWidget *parent = 0);
    ~Dialog_text_input();

    void setup(QString new_window_title, QString new_labeltext);
    void set_infotext(QString new_labeltext);
    void set_default_input(QString def);

signals:
    void signal_userinput(QString msg);
    void signal_cancel();

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();

private:
    Ui::Dialog_text_input *ui;
    void closeEvent(QCloseEvent *ce);
};

#endif // DIALOG_TEXT_INPUT_H
