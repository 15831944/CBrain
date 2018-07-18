#ifndef DIALOG_LAGER_H
#define DIALOG_LAGER_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "text.h"

namespace Ui {
class Dialog_lager;
}

class Dialog_lager : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_lager(QWidget *parent = 0);
    ~Dialog_lager();

    void set_db(cbrainbatabase *new_db);
    void setup();

signals:
    void signal_send_data(text_zeilenweise data);

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

    void on_lineEdit_artikelfilter_textChanged(const QString &arg1);

    void on_lineEdit_komfilter_textChanged(const QString &arg1);

private:
    Ui::Dialog_lager *ui;

    cbrainbatabase *dbeigen;
    QString current_id;

    text_zeilenweise artikel;
    text_zeilenweise projekte;

    void clear();
};

#endif // DIALOG_LAGER_H
