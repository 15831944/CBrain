#include "dialog_tableeditor.h"
#include "ui_dialog_tableeditor.h"

Dialog_tableeditor::Dialog_tableeditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_tableeditor)
{
    ui->setupUi(this);
}

Dialog_tableeditor::~Dialog_tableeditor()
{
    delete ui;
}

void Dialog_tableeditor::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Dialog_tableeditor::set_table(QString t)
{
    table = t;
    zeilen_id = dbeigen->get_data_tz(table, "id");
    update_gui();
    combobox_default_setup();
}

void Dialog_tableeditor::update_gui()
{
    ui->listWidget_zeilen->clear();
    for(uint i=1; i<=zeilen_id.zeilenanzahl() ;i++)
    {
        ui->listWidget_zeilen->addItem(zeilen_id.zeile(i));
    }

    text_zeilenweise pa = dbeigen->get_param_tz(table);
    ui->comboBox_param->clear();
    for(uint i=2; i<=pa.zeilenanzahl() ;i++)//erst ab Wert 2 weil Werti 1 == id -> id darf nicht verändert werden
    {
        ui->comboBox_param->addItem(pa.zeile(i));
        ui->comboBox_param2->addItem(pa.zeile(i));
        ui->comboBox_param3->addItem(pa.zeile(i));
        ui->comboBox_param4->addItem(pa.zeile(i));
        ui->comboBox_param5->addItem(pa.zeile(i));
        ui->comboBox_param6->addItem(pa.zeile(i));
        ui->comboBox_param7->addItem(pa.zeile(i));
        ui->comboBox_param8->addItem(pa.zeile(i));
        ui->comboBox_param9->addItem(pa.zeile(i));
    }
}

void Dialog_tableeditor::combobox_default_setup()
{
    int zeilanz = dbeigen->get_param_tz(table).zeilenanzahl()-1;
    if(zeilanz >= 2)
    {
        ui->comboBox_param2->setCurrentIndex(2-1);
    }
    if(zeilanz >= 3)
    {
        ui->comboBox_param3->setCurrentIndex(3-1);
    }
    if(zeilanz >= 4)
    {
        ui->comboBox_param4->setCurrentIndex(4-1);
    }
    if(zeilanz >= 5)
    {
        ui->comboBox_param5->setCurrentIndex(5-1);
    }
    if(zeilanz >= 6)
    {
        ui->comboBox_param6->setCurrentIndex(6-1);
    }
    if(zeilanz >= 7)
    {
        ui->comboBox_param7->setCurrentIndex(7-1);
    }
    if(zeilanz >= 8)
    {
        ui->comboBox_param8->setCurrentIndex(8-1);
    }
    if(zeilanz >= 9)
    {
        ui->comboBox_param9->setCurrentIndex(9-1);
    }
}

void Dialog_tableeditor::on_pushButton_ok_clicked()
{
    this->close();
}

void Dialog_tableeditor::on_listWidget_zeilen_currentRowChanged()
{
    on_comboBox_param_currentIndexChanged(0);
    on_comboBox_param2_currentIndexChanged(0);
    on_comboBox_param3_currentIndexChanged(0);
    on_comboBox_param4_currentIndexChanged(0);
    on_comboBox_param5_currentIndexChanged(0);
    on_comboBox_param6_currentIndexChanged(0);
    on_comboBox_param7_currentIndexChanged(0);
    on_comboBox_param8_currentIndexChanged(0);
    on_comboBox_param9_currentIndexChanged(0);
}

void Dialog_tableeditor::on_pushButton_new_clicked()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param->count() > 0)
        {
            dbeigen->data_new(table);
            zeilen_id = dbeigen->get_data_tz(table, "id");
            update_gui();
            if(zeilen_id.zeilenanzahl() >= 1)
            {
                ui->listWidget_zeilen->setCurrentRow(zeilen_id.zeilenanzahl()-1);
            }
        }
    }
}

void Dialog_tableeditor::on_pushButton_del_clicked()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param->count() > 0)
        {
            dbeigen->data_del(table, ui->listWidget_zeilen->currentItem()->text());
            zeilen_id = dbeigen->get_data_tz(table, "id");
            update_gui();
            if(zeilen_id.zeilenanzahl() >= 1)
            {
                ui->listWidget_zeilen->setCurrentRow(zeilen_id.zeilenanzahl()-1);
            }
        }
    }
}

void Dialog_tableeditor::on_comboBox_param_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param->count() > 0)
        {
            ui->lineEdit_value->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param2_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param2->count() > 0)
        {
            ui->lineEdit_value2->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param2->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param3_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param3->count() > 0)
        {
            ui->lineEdit_value3->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param3->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param4_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param4->count() > 0)
        {
            ui->lineEdit_value4->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param4->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param5_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param5->count() > 0)
        {
            ui->lineEdit_value5->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param5->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param6_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param6->count() > 0)
        {
            ui->lineEdit_value6->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param6->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param7_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param7->count() > 0)
        {
            ui->lineEdit_value7->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param7->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param8_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param8->count() > 0)
        {
            ui->lineEdit_value8->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param8->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}
void Dialog_tableeditor::on_comboBox_param9_currentIndexChanged(int index)
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param9->count() > 0)
        {
            ui->lineEdit_value9->setText(dbeigen->get_data_qstring(table, \
                                                                  ui->comboBox_param9->currentText(),\
                                                                  ui->listWidget_zeilen->currentItem()->text()));
        }
    }
}

void Dialog_tableeditor::on_lineEdit_value_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param->currentText(),\
                               ui->lineEdit_value->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value2_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param2->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param2->currentText(),\
                               ui->lineEdit_value2->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value3_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param3->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param3->currentText(),\
                               ui->lineEdit_value3->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value4_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param4->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param4->currentText(),\
                               ui->lineEdit_value4->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value5_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param5->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param5->currentText(),\
                               ui->lineEdit_value5->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value6_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param6->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param6->currentText(),\
                               ui->lineEdit_value6->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value7_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param7->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param7->currentText(),\
                               ui->lineEdit_value7->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value8_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param8->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param8->currentText(),\
                               ui->lineEdit_value8->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
void Dialog_tableeditor::on_lineEdit_value9_editingFinished()
{
    if(ui->listWidget_zeilen->count() > 0  &&  ui->listWidget_zeilen->currentRow() >= 0)
    {
        if(ui->comboBox_param9->count() > 0)
        {
            dbeigen->data_edit(table,\
                               ui->comboBox_param9->currentText(),\
                               ui->lineEdit_value9->text(),\
                               ui->listWidget_zeilen->currentItem()->text());
        }
    }
}
