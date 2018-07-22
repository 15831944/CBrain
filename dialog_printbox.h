#ifndef DIALOG_PRINTBOX_H
#define DIALOG_PRINTBOX_H

#include <QDialog>
#include <QFileDialog>
#include <QPrintDialog>
#include <QDebug>
#include <QtGui>
#include <QtCore>

namespace Ui {
class Dialog_printbox;
}

class Dialog_printbox : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_printbox(QWidget *parent = 0);
    ~Dialog_printbox();

    void setText(QString text);

private slots:
    void on_pushButton_print_clicked();

private:
    Ui::Dialog_printbox *ui;
};

#endif // DIALOG_PRINTBOX_H
