#include "dialog_artikel_verwenden.h"
#include "ui_dialog_artikel_verwenden.h"

Dialog_artikel_verwenden::Dialog_artikel_verwenden(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_artikel_verwenden)
{
    ui->setupUi(this);
}

Dialog_artikel_verwenden::~Dialog_artikel_verwenden()
{
    delete ui;
}

void Dialog_artikel_verwenden::on_pushButton_cancel_clicked()
{
    this->close();
    emit signal_cancel();
}

void Dialog_artikel_verwenden::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
    if(dbeigen != NULL)
    {
        status_tz.clear();
        status_id_tz.clear();
        status_tz = dbeigen->get_data_tz(TABNAME_STATUS, PARAM_STATUS_STATUS);
        status_id_tz = dbeigen->get_data_tz(TABNAME_STATUS, PARAM_STATUS_ID);
        ui->comboBox_status->clear();
        for(uint i=1; i<=status_tz.zeilenanzahl() ;i++)
        {
            ui->comboBox_status->addItem(status_tz.zeile(i));
        }
    }
}

void Dialog_artikel_verwenden::set_artikel_id(QString id)
{
    ui->lineEdit_id->setText(id);
}

void Dialog_artikel_verwenden::on_lineEdit_id_textChanged(const QString &arg1)
{
    if(dbeigen != NULL)
    {
        QString nr, bez, beziehungen;
        nr = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_NR, arg1);
        bez = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZ, arg1);
        beziehungen = dbeigen->get_data_qstring(TABNAME_ARTIKEL, PARAM_ARTIKEL_BEZIEHUNG, arg1);
        beziehungen.replace("#br#", "\n");
        text_zeilenweise beziehungen_tz;
        beziehungen_tz.set_text(beziehungen);

        ui->lineEdit_nr->setText(nr);
        ui->lineEdit_bez->setText(bez);

        bezi_id_tz.clear();
        bezi_nr_tz.clear();
        bezi_bez_tz.clear();
        bezi_default_tz.clear();
        bezi_menge_tz.clear();
        for(uint i=1; i<=beziehungen_tz.zeilenanzahl() ;i++)
        {
            QString zeile = beziehungen_tz.zeile(i);
            QString tmp = text_links(zeile, " |||");
            bezi_id_tz.zeile_anhaengen(text_links(zeile, " | "));
            bezi_nr_tz.zeile_anhaengen(text_mitte(tmp, " | ", " | "));
            QString tmpbez = text_rechts(tmp, " | ");
            tmpbez = text_rechts(tmpbez, " | ");
            bezi_bez_tz.zeile_anhaengen(tmpbez);
            bezi_default_tz.zeile_anhaengen(text_mitte(zeile, " |||", " |||").replace(" ", ""));
            QString tmpmenge = text_rechts(zeile, " ||| ");
            tmpmenge = text_rechts(tmpmenge, " ||| ");
            bezi_menge_tz.zeile_anhaengen(tmpmenge);
        }

        ui->listWidget->clear();
        QListWidgetItem* item[beziehungen_tz.zeilenanzahl()];
        for(uint i=1; i<=beziehungen_tz.zeilenanzahl() ;i++)
        {
            QString itemname;
            itemname += bezi_default_tz.zeile(i);
            itemname += " | ";
            itemname += bezi_nr_tz.zeile(i);
            itemname += "   ";
            itemname += bezi_bez_tz.zeile(i);
            item[i-1] = new QListWidgetItem(itemname  , ui->listWidget);
            item[i-1]->setFlags(item[i-1]->flags() | Qt::ItemIsUserCheckable); // set checkable flag
            QString item_is_checked = bezi_default_tz.zeile(i);
            if(item_is_checked == "1")
            {
                item[i-1]->setCheckState(Qt::Checked); // AND initialize check state
            }else
            {
                item[i-1]->setCheckState(Qt::Unchecked); // AND initialize check state
            }
            ui->listWidget->addItem(item[i-1]);
        }

    }
}

void Dialog_artikel_verwenden::on_pushButton_ok_clicked()
{
    text_zeilenweise data, beziehungen;

    data.zeile_anhaengen(ui->lineEdit_id->text());                                        //Wert 1 = id
    data.zeile_anhaengen(float_to_qstring(ui->doubleSpinBox_menge->value()));             //Wert 2 = Menge
    data.zeile_anhaengen(status_id_tz.zeile(ui->comboBox_status->currentIndex()+1));      //Wert 3 = Status

    for(int i=1; i<=ui->listWidget->count() ;i++)
    {
        if(ui->listWidget->item(i-1)->checkState() == Qt::Checked)
        {
            QString zeile;
            zeile += bezi_id_tz.zeile(i);
            zeile += " ||| ";
            zeile += bezi_menge_tz.zeile(i);
            beziehungen.zeile_anhaengen(zeile);
        }
    }
    data.zeile_anhaengen(beziehungen.get_text().replace("\n", NEW_LINE_BR));
    this->close();
    emit signal_send_data(data);
}

void Dialog_artikel_verwenden::set_data(text_zeilenweise data)
{
    if(!ui->lineEdit_id->text().isEmpty())
    {
        //Wert 1 = Menge
        //Wert 2 = Status-ID
        QString menge = data.zeile(1);
        QString status_id = data.zeile(2);
        QString beziehungen = data.zeile(3);
        beziehungen.replace(NEW_LINE_BR, "\n");

        text_zeilenweise beziehungen_tz;
        beziehungen_tz.set_text(beziehungen);
        text_zeilenweise beziehungen_tz_ids;
        //text_zeilenweise beziehungen_tz_menge;
        for(uint i=1; i<=beziehungen_tz.zeilenanzahl() ;i++)
        {
            QString zeile = beziehungen_tz.zeile(i);
            beziehungen_tz_ids.zeile_anhaengen(text_links(zeile, " ||| "));
            //beziehungen_tz_menge.zeile_anhaengen(text_rechts(zeile, " ||| "));
        }

        ui->doubleSpinBox_menge->setValue(menge.toDouble());
        int row = ui->comboBox_status->findText(dbeigen->get_data_qstring(TABNAME_STATUS, PARAM_STATUS_STATUS, status_id));
        ui->comboBox_status->setCurrentIndex(row);

        for(int i=1; i<=ui->listWidget->count() ;i++)
        {
            bool zeile_is_checked = false;
            for(uint ii=1; ii<=beziehungen_tz_ids.zeilenanzahl() ;ii++)
            {
                if(bezi_id_tz.zeile(i) == beziehungen_tz_ids.zeile(ii))
                {
                    zeile_is_checked = true;
                    break;
                }
            }
            if(zeile_is_checked)
            {
                ui->listWidget->item(i-1)->setCheckState(Qt::Checked); // AND initialize check stat
            }else
            {
                ui->listWidget->item(i-1)->setCheckState(Qt::Unchecked); // AND initialize check state
            }
        }
    }
}













