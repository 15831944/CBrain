#ifndef DIALOG_PROJEKTE_H
#define DIALOG_PROJEKTE_H

#include <QDialog>
#include <QMessageBox>

#include "text_zeilenweise.h"
#include "cbrainbatabase.h"
#include "_tabellennamen.h"

namespace Ui {
class Dialog_projekte;
}

class Dialog_projekte : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_projekte(QWidget *parent = 0);
    ~Dialog_projekte();

    void set_data(text_zeilenweise daten, QString id);

signals:
    void signal_send_data(text_zeilenweise data);
    void signal_send_data(text_zeilenweise data, QString id);
    void signal_cancel();

private slots:
    void on_pushButton_ok_clicked();
    void on_pushButton_cancel_clicked();

private:
    Ui::Dialog_projekte *ui;

    QString current_id;

    void clear();
    void closeEvent(QCloseEvent *ce);
};

#endif // DIALOG_PROJEKTE_H
