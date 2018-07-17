#include "dialog_lager.h"
#include "ui_dialog_lager.h"

Dialog_lager::Dialog_lager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_lager)
{
    ui->setupUi(this);
}

Dialog_lager::~Dialog_lager()
{
    delete ui;
}
