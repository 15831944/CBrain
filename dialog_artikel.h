#ifndef DIALOG_ARTIKEL_H
#define DIALOG_ARTIKEL_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "cbrainbatabase.h"
#include "_tabellennamen.h"
#include "text.h"

namespace Ui {
class Dialog_artikel;
}

class Dialog_artikel : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_artikel(QWidget *parent = 0);
    ~Dialog_artikel();

    void set_db(cbrainbatabase *new_db);
    void setup();
    void set_data(text_zeilenweise daten, QString id);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_send_data(text_zeilenweise data, QString id);
    void signal_cancel();

private slots:
    void on_pushButton_cancel_clicked();
    void on_pushButton_ok_clicked();
    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::Dialog_artikel *ui;

    cbrainbatabase *dbeigen;
    QString current_id;

    text_zeilenweise lieferanten;

    void clear();
};

#endif // DIALOG_ARTIKEL_H
