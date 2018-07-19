#include "form_backup.h"
#include "ui_form_backup.h"

Form_backup::Form_backup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_backup)
{
    ui->setupUi(this);
}

Form_backup::~Form_backup()
{
    delete ui;
}

void Form_backup::resizeEvent(QResizeEvent *event)
{
    //int hoehe = this->geometry().height();
    int breite = this->geometry().width();

    ui->pushButton_backup->move(1,1);
    ui->pushButton_restore->move(1, 3 + ui->pushButton_backup->height());

    ui->lineEdit_backupto->move(2+ ui->pushButton_backup->width(),1);
    ui->lineEdit_restorefrom->move(2+ ui->pushButton_backup->width() , 3 + ui->pushButton_backup->height() );

    ui->lineEdit_backupto->setFixedHeight(ui->pushButton_backup->height());
    ui->lineEdit_restorefrom->setFixedHeight(ui->pushButton_backup->height());

    int tmpb = breite - (1+ui->pushButton_backup->width()-1-1);
    if(tmpb < 1)
    {
        tmpb = 1;
    }
    ui->lineEdit_backupto->setFixedWidth(tmpb);
    ui->lineEdit_restorefrom->setFixedWidth(tmpb);


    QWidget::resizeEvent(event);
}

void Form_backup::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_backup::show()
{
    setVisible(true);
}

void Form_backup::on_pushButton_backup_clicked()
{
    QString msg;
    QString ezpar = "\t";

    text_zeilenweise tables;
    tables = dbeigen->get_tables_tz();

    for(uint i=1; i<= tables.zeilenanzahl() ;i++)
    {
        text_zeilenweise param = dbeigen->get_param_tz(tables.zeile(i));
        text_zeilenweise param_type = dbeigen->get_param_type_tz(tables.zeile(i));
        text_zeilenweise param_primkey = dbeigen->get_param_primkey_tz(tables.zeile(i));
        text_zeilenweise param_extra = dbeigen->get_param_extra_tz(tables.zeile(i));
        text_zeilenweise param_notnull = dbeigen->get_param_notnull_tz(tables.zeile(i));
        text_zeilenweise param_default = dbeigen->get_param_default_tz(tables.zeile(i));

        msg += "<BEGIN_Tabelle>";
        msg += tables.zeile(i);
        msg += "\n";

        for(uint ii=1; ii<=param.zeilenanzahl() ;ii++)
        {
            msg += "<Name>";
            msg += param.zeile(ii);
            msg += ezpar;

            msg += "<TYP>";
            msg += param_type.zeile(ii);
            msg += ezpar;

            msg += "<NotNull>";
            msg += param_notnull.zeile(ii);
            msg += ezpar;

            msg += "<IsPri>";
            msg += param_primkey.zeile(ii);
            msg += ezpar;

            msg += "<Default>";
            msg += param_default.zeile(ii);
            msg += ezpar;

            msg += "<Extra>";
            msg += param_extra.zeile(ii);
            msg += ezpar;

            msg += "\n";
        }

        msg += "<ENDE_Tabelle>";
        msg += tables.zeile(i);
        msg += "\n";

    }

    //msg in Datei speichern:
    QMessageBox mb;
    mb.setText(msg);
    mb.exec();
}
