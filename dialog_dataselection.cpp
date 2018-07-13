#include "dialog_dataselection.h"
#include "ui_dialog_dataselection.h"

Dialog_dataselection::Dialog_dataselection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_dataselection)
{
    ui->setupUi(this);
}

Dialog_dataselection::~Dialog_dataselection()
{
    delete ui;
}

void Dialog_dataselection::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_dataselection::set_data(text_zeilenweise data)
{
    ui->listWidget->clear();

    QListWidgetItem* item[data.zeilenanzahl()];
    for(uint i=1; i<=data.zeilenanzahl() ;i++)
    {
        item[i-1] = new QListWidgetItem(data.zeile(i) , ui->listWidget);
        item[i-1]->setFlags(item[i-1]->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item[i-1]->setCheckState(Qt::Unchecked); // AND initialize check state
        ui->listWidget->addItem(item[i-1]);
    }
}

void Dialog_dataselection::set_data(text_zeilenweise data, text_zeilenweise id)
{
    set_data(data);
    ids = id;
}

void Dialog_dataselection::on_pushButton_ok_clicked()
{
    text_zeilenweise current_ids;

    for(int i=1; i<=ui->listWidget->count() ;i++)
    {
        if(ui->listWidget->item(i-1)->checkState() == Qt::Checked)
        {
            //tz.zeile_anhaengen(ui->listWidget->item(i-1)->text());
            current_ids.zeile_anhaengen(ids.zeile(i));
        }
    }
    this->close();

    emit signal_send_selection(current_ids);
}

void Dialog_dataselection::on_pushButton_all_clicked()
{
    for(int i=1; i<=ui->listWidget->count() ;i++)
    {
        ui->listWidget->item(i-1)->setCheckState(Qt::Checked);
    }
}

void Dialog_dataselection::on_pushButton_non_clicked()
{
    for(int i=1; i<=ui->listWidget->count() ;i++)
    {
        ui->listWidget->item(i-1)->setCheckState(Qt::Unchecked);
    }
}

