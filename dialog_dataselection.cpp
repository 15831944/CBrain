#include "dialog_dataselection.h"
#include "ui_dialog_dataselection.h"

Dialog_dataselection::Dialog_dataselection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_dataselection)
{
    ui->setupUi(this);
    anzretwerte = 0;
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Dialog_dataselection::~Dialog_dataselection()
{
    delete ui;
}

void Dialog_dataselection::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);    
}

void Dialog_dataselection::on_pushButton_cancel_clicked()
{
    this->close();
    emit signal_cancel();
}

void Dialog_dataselection::set_data(text_zeilenweise data)
{
    mydata = data;
    update_listwidget();
}
void Dialog_dataselection::set_data(text_zeilenweise data, text_zeilenweise id)
{
    ids = id;
    mydata = data;
    update_listwidget();
}

void Dialog_dataselection::on_lineEdit_filter_textChanged()
{
    update_listwidget();
}

void Dialog_dataselection::update_listwidget()
{
    ui->listWidget->clear();
    filtered_data.clear();
    filtered_ids.clear();

    if(ui->lineEdit_filter->text().isEmpty())
    {
        filtered_data = mydata;
        filtered_ids = ids;
    }else
    {
        for(uint i=1; i<=mydata.zeilenanzahl() ;i++)
        {
            if(mydata.zeile(i).toUpper().contains(ui->lineEdit_filter->text().toUpper()))
            {
                filtered_data.zeile_anhaengen(mydata.zeile(i));
                if(ids.zeilenanzahl() != 0)
                {
                    filtered_ids.zeile_anhaengen(ids.zeile(i));
                }
            }
        }
    }
    QListWidgetItem* item[filtered_data.zeilenanzahl()];
    for(uint i=1; i<=filtered_data.zeilenanzahl() ;i++)
    {
        item[i-1] = new QListWidgetItem(filtered_data.zeile(i) , ui->listWidget);
        item[i-1]->setFlags(item[i-1]->flags() | Qt::ItemIsUserCheckable); // set checkable flag
        item[i-1]->setCheckState(Qt::Unchecked); // AND initialize check state
        ui->listWidget->addItem(item[i-1]);
    }
}

void Dialog_dataselection::on_pushButton_ok_clicked()
{
    text_zeilenweise current_ids;

    if(filtered_ids.zeilenanzahl() == 1)
    {
        current_ids = filtered_ids;
    }else
    {
        for(int i=1; i<=ui->listWidget->count() ;i++)
        {
            if(ui->listWidget->item(i-1)->checkState() == Qt::Checked)
            {
                //tz.zeile_anhaengen(ui->listWidget->item(i-1)->text());
                current_ids.zeile_anhaengen(filtered_ids.zeile(i));
            }
        }
    }

    if(anzretwerte != 0)
    {
        if(anzretwerte == current_ids.zeilenanzahl())
        {
            this->close();
            emit signal_send_selection(current_ids);
        }else
        {
            QString msg;
            if(anzretwerte == 1)
            {
                msg += "Bitte nur einen Eintrag!";
            }else
            {
                msg += "Bitte ";
                msg += int_to_qstring(anzretwerte);
                msg += " Eintraege auswaelen!";
            }


            QMessageBox mb;
            mb.setText(msg);
            mb.exec();
        }
    }else
    {
        this->close();
        emit signal_send_selection(current_ids);
    }
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

void Dialog_dataselection::set_anz_returnwerte(int anz)
{
    anzretwerte = anz;
}
