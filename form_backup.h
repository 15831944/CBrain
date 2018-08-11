#ifndef FORM_BACKUP_H
#define FORM_BACKUP_H

#include <QWidget>
#include <QFile>

#include "cbrainbatabase.h"
#include "_tabname_promatpos.h"
#include "_tabname_promat.h"
#include "_tabname_promatposlist.h"
#include "inifile.h"
#include "text.h"

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
    void set_ini(inifile *in);

signals:
    void signal_save_ini();

protected:
    void resizeEvent(QResizeEvent *event);

private slots:
    void on_pushButton_backup_clicked();
    void on_pushButton_restore_clicked();
    void on_lineEdit_backupto_editingFinished();
    void on_lineEdit_restorefrom_editingFinished();

    void on_pushButton_backup_all_clicked();

private:
    Ui::Form_backup *ui;

    cbrainbatabase *dbeigen;
    inifile *ini;
};

#endif // FORM_BACKUP_H
