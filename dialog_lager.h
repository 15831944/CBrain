#ifndef DIALOG_LAGER_H
#define DIALOG_LAGER_H

#include <QDialog>

namespace Ui {
class Dialog_lager;
}

class Dialog_lager : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_lager(QWidget *parent = 0);
    ~Dialog_lager();

private:
    Ui::Dialog_lager *ui;
};

#endif // DIALOG_LAGER_H
