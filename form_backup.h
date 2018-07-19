#ifndef FORM_BACKUP_H
#define FORM_BACKUP_H

#include <QWidget>

#include "cbrainbatabase.h"

namespace Ui {
class Form_backup;
}

class Form_backup : public QWidget
{
    Q_OBJECT

public:
    explicit Form_backup(QWidget *parent = 0);
    ~Form_backup();

    void set_db(cbrainbatabase *new_db);
    void show();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButton_backup_clicked();

private:
    Ui::Form_backup *ui;

    cbrainbatabase *dbeigen;
};

#endif // FORM_BACKUP_H
