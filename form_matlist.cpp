#include "form_matlist.h"
#include "ui_form_matlist.h"

Form_matlist::Form_matlist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_matlist)
{
    ui->setupUi(this);
}

Form_matlist::~Form_matlist()
{
    delete ui;
}

void Form_matlist::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Form_matlist::set_user(QString u)
{
    user = u;
}

void Form_matlist::resizeEvent(QResizeEvent *event)
{
    int hoehe = this->height();
    int breite = this->width();

    int b_li = breite/5*3;
    int b_re = breite - b_li - 4;   //Rand_li = 1 + Rand_re = 1 + Rand_mi = 2 == 4

    int h_btn = ui->pushButton_projektauswahl->height();

    //links Kopfzeile:
    ui->lineEdit_projekt->setFixedHeight(h_btn);
    ui->lineEdit_projekt->move(1,1);

    ui->pushButton_projektauswahl->move(1+ui->lineEdit_projekt->width()+1,\
                                        1);

    ui->lineEdit_filter->setFixedHeight(h_btn);
    ui->lineEdit_filter->move(b_li - ui->lineEdit_filter->width(),\
                              1);
    ui->label_filter->setFixedHeight(h_btn);
    ui->label_filter->move(b_li - ui->lineEdit_filter->width() - 1 - ui->label_filter->width(),\
                           1);

    //links Tabelle:
    ui->tableView->setFixedSize(b_li,\
                                hoehe - 1 - h_btn - 1 - 1);
    ui->tableView->move(1,\
                        1 + h_btn + 1);

    //rechts Kopfzeile:
    ui->pushButton_check_all_pos->setFixedWidth(b_re/2-2);
    ui->pushButton_check_all_pos->move(1 + b_li + 2,\
                                       1);

    ui->pushButton_check_no_pos->setFixedWidth(ui->pushButton_check_all_pos->width());
    ui->pushButton_check_no_pos->move(1 + b_li + 2 + ui->pushButton_check_all_pos->width() + 1,\
                                      1);

    //rechts Listwidget + Buttons
    ui->listWidget_matpos->setFixedSize(b_re,\
                                        hoehe - 1*5 - h_btn*3);
    ui->listWidget_matpos->move(1 + b_li + 2,\
                                1 + h_btn + 1);

    ui->pushButton_pos_new->setFixedWidth(b_re/3 - 2*2);
    ui->pushButton_pos_edit->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_delete->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_copy->setFixedWidth(ui->pushButton_pos_new->width());
    ui->pushButton_pos_import->setFixedWidth(ui->pushButton_pos_new->width());

    ui->pushButton_pos_new->move(1 + b_li + 2,\
                                 1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_edit->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                  1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_delete->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                    1 + h_btn + 1 + ui->listWidget_matpos->height() + 1);
    ui->pushButton_pos_copy->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1),\
                                  1 + h_btn + 1 + ui->listWidget_matpos->height() + 1 + h_btn + 1);
    ui->pushButton_pos_import->move(1 + b_li + 2 + (ui->pushButton_pos_new->width()+1)*2,\
                                    1 + h_btn + 1 + ui->listWidget_matpos->height() + 1 + h_btn + 1);

    QWidget::resizeEvent(event);
}




















