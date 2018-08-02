#include "dialog_artikel.h"
#include "ui_dialog_artikel.h"

Dialog_artikel::Dialog_artikel(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_artikel)
{
    ui->setupUi(this);
    current_id = "0";
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
}

Dialog_artikel::~Dialog_artikel()
{
    delete ui;
}

void Dialog_artikel::setup()
{
    //Unbedingt vorher Pointer auf DB zuweisen, sonst Programmabsturz!
    clear();
    //text_zeilenweise lieferanten;
    //lieferanten aus Tabelle einlesen:
    if(dbeigen != NULL)
    {
        QSqlDatabase db;

        db = QSqlDatabase::database("dbglobal");
        db.setHostName(dbeigen->get_host());
        db.setDatabaseName(dbeigen->get_dbname());
        db.setUserName(dbeigen->get_user());
        db.setPassword(dbeigen->get_pwd());

        if(db.open())
        {
            QSqlQuery q(db);
            QString cmd;
            cmd += "SELECT ";
            cmd += PARAM_LIEFERANT_ID;
            cmd += ", ";
            cmd += PARAM_LIEFERANT_NAME;
            cmd += " FROM ";
            cmd += TABNAME_LIEFERANT;

            if(q.exec(cmd))
            {
                while(q.next())
                {
                    QString tmp;
                    tmp += q.value(0).toString();
                    tmp += " / ";
                    tmp += q.value(1).toString();
                    lieferanten.zeile_anhaengen(tmp);
                }
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler:\n" + q.lastError().text());
                mb.exec();
            }
            db.close();

        }else
        {
            QMessageBox mb;
            mb.setText(tr("Fehler bei Datenbankverbindung!"));
            mb.exec();
        }
    }

    ui->comboBox_lieferant->addItem("---");
    for(uint i=1; i<=lieferanten.zeilenanzahl() ;i++)
    {
        ui->comboBox_lieferant->addItem(lieferanten.zeile(i));
    }
}

void Dialog_artikel::set_data(text_zeilenweise daten, QString id)
{
    current_id = id;
    set_data(daten);
}

void Dialog_artikel::set_data(text_zeilenweise daten)
{
    ui->lineEdit_artielnummer->setText(daten.zeile(1));
    ui->lineEdit_bezeichnung->setText(daten.zeile(2));

    QString tmp;
    tmp += daten.zeile(3);
    tmp += " / ";
    tmp += dbeigen->get_data_qstring(TABNAME_LIEFERANT, PARAM_LIEFERANT_NAME, daten.zeile(3));

    ui->comboBox_lieferant->setCurrentIndex(ui->comboBox_lieferant->findText(tmp));

    ui->lineEdit_lagerort->setText(daten.zeile(4));
    ui->spinBox_ve->setValue(daten.zeile(5).toInt());
    ui->lineEdit_komment->setText(daten.zeile(6));

    QString beziehungen = daten.zeile(7);
    if(beziehungen != "---")//wenn es Beziehungen gibt
    {
        beziehungen.replace("#br#", "\n");
        ui->listWidget_beziehung->clear();
        text_zeilenweise beziehungen_tz;
        beziehungen_tz.set_text(beziehungen);
        for(uint i=1; i<=beziehungen_tz.zeilenanzahl() ;i++)
        {
            ui->listWidget_beziehung->addItem(beziehungen_tz.zeile(i));
        }
    }
    ui->doubleSpinBox_preis->setValue(daten.zeile(8).toDouble());
    if(daten.zeile(9) == "1")
    {
        ui->checkBox_isfavorit->setChecked(true);
    }else
    {
        ui->checkBox_isfavorit->setChecked(false);
    }
}

void Dialog_artikel::clear()
{
    ui->lineEdit_artielnummer->clear();
    ui->lineEdit_bezeichnung->clear();
    ui->comboBox_lieferant->clear();
}

void Dialog_artikel::set_db(cbrainbatabase *new_db)
{
    dbeigen = new_db;
}

void Dialog_artikel::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);
}

//----------------------------------Buttons:
void Dialog_artikel::on_pushButton_cancel_clicked()
{
    this->close();
    emit signal_cancel();
}

void Dialog_artikel::on_pushButton_ok_clicked()
{
    if( ui->lineEdit_artielnummer->text().isEmpty()     ||\
        ui->lineEdit_bezeichnung->text().isEmpty()      )
    {
        QMessageBox mb;
        mb.setText(tr("Die Datenfelder \"Artikelnummer\" und \"Bezeichnung\" dürfen nicht leer sein!"));
        mb.exec();
    }else if( ui->comboBox_lieferant->currentText() == "---")
    {
        QMessageBox mb;
        mb.setText(tr("Bitte zuerst einen Lieferanten eintragen!"));
        mb.exec();
    }else
    {
        //------------------------------------------------------------------------
        text_zeilenweise tz;
        tz.zeile_anhaengen(ui->lineEdit_artielnummer->text());      //Wert 1
        tz.zeile_anhaengen(ui->lineEdit_bezeichnung->text());       //Wert 2
        QString lief = text_links(ui->comboBox_lieferant->currentText(), " / ");
        tz.zeile_anhaengen(lief);                                   //Wert 3
        tz.zeile_anhaengen(ui->lineEdit_lagerort->text());          //Wert 4
        tz.zeile_anhaengen(int_to_qstring(ui->spinBox_ve->value()));//Wert 5
        tz.zeile_anhaengen(ui->lineEdit_komment->text());           //Wert 6
        QString beziehungen;
        for(int i=1; i<=ui->listWidget_beziehung->count() ;i++)
        {
            beziehungen += ui->listWidget_beziehung->item(i-1)->text();
            if(i!=ui->listWidget_beziehung->count())
            {
                beziehungen += "#br#";
                //es darf kein \n enthalten sein weil später durch tz verarbeitet wird
            }
        }
        tz.zeile_anhaengen(beziehungen);                            //Wert 7
        tz.zeile_anhaengen(double_to_qstring(ui->doubleSpinBox_preis->value()));    //Wert 8
        if(ui->checkBox_isfavorit->isChecked())
        {
            tz.zeile_anhaengen("1");                                //Wert 9
        }else
        {
            tz.zeile_anhaengen("0");                                //Wert 9
        }

        //------------------------------------------------------------------------
        this->close();
        if(current_id == "0")
        {
            emit signal_send_data(tz);
        }else
        {
            emit signal_send_data(tz, current_id);
        }
    }
}

void Dialog_artikel::on_pushButton_beziehung_new_clicked()
{
    Dialog_artikel_beziehung *d = new Dialog_artikel_beziehung(this);
    d->setWindowTitle("Artikelbeziehung anlegen");
    d->set_db(dbeigen);
    d->set_artikelid_eigen(current_id);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),      \
            this, SLOT(slot_beziehung_new(text_zeilenweise))    );
    d->exec();
    delete d;
}

void Dialog_artikel::on_pushButton_beziehung_delete_clicked()
{
    if(ui->listWidget_beziehung->currentRow() != -1)
    {
        if(ui->listWidget_beziehung->count() == 1)
        {
            ui->listWidget_beziehung->clear();
        }else
        {
            delete ui->listWidget_beziehung->currentItem();
        }
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte erst eine Zeile auswählen"));
        mb.exec();
    }
}

void Dialog_artikel::on_pushButton_beziehung_edit_clicked()
{
    if(ui->listWidget_beziehung->currentRow() != -1)
    {
        QString zeile = ui->listWidget_beziehung->currentItem()->text();
        QString id              = text_links(zeile, " ||| ");
        id = text_links(id, " | ");
        QString defaultstatus   = text_mitte(zeile , " ||| ", " ||| ");
        QString menge           = text_rechts(zeile, " ||| ");
        menge = text_rechts(menge, " ||| ");

        text_zeilenweise data;
        data.zeile_anhaengen(id);
        data.zeile_anhaengen(defaultstatus);
        data.zeile_anhaengen(menge);

        Dialog_artikel_beziehung *d = new Dialog_artikel_beziehung(this);
        d->setWindowTitle("Artikelbeziehung bearbeiten");
        d->set_db(dbeigen);
        d->set_artikelid_eigen(current_id);
        d->set_data(data);
        d->set_id_const(true);
        connect(d, SIGNAL(signal_send_data(text_zeilenweise)),      \
                this, SLOT(slot_beziehung_edit(text_zeilenweise))    );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Bitte erst eine Zeile auswählen"));
        mb.exec();
    }
}

//----------------------------------public slots:
void Dialog_artikel::slot_beziehung_new(text_zeilenweise data)
{
    QString artikel         = data.zeile(1);
    QString id              = text_links(artikel, " | ");
    QString defaultstatus   = data.zeile(2);
    QString menge           = data.zeile(3);

    bool isvalid = true;

    for(int i=1; i<=ui->listWidget_beziehung->count() ;i++)
    {
        QString tmp = ui->listWidget_beziehung->item(i-1)->text();
        tmp = text_links(tmp, " ||| ");
        if(tmp == id)
        {
            isvalid = false;
            break;
        }
    }
    if(isvalid == true)
    {
        QString msg;
        msg += artikel;
        msg += " ||| ";
        msg += defaultstatus;
        msg += " ||| ";
        msg += menge;

        ui->listWidget_beziehung->addItem(msg);
    }else
    {
        QMessageBox mb;
        mb.setText(tr("Dieser Artikel steht bereits in Beziehung!"));
        mb.exec();
    }
}

void Dialog_artikel::slot_beziehung_edit(text_zeilenweise data)
{
    QString artikel         = data.zeile(1);
    QString id              = text_links(artikel, " | ");
    QString defaultstatus   = data.zeile(2);
    QString menge           = data.zeile(3);

    QString msg;
    msg += artikel;
    msg += " ||| ";
    msg += defaultstatus;
    msg += " ||| ";
    msg += menge;

    ui->listWidget_beziehung->currentItem()->setText(msg);

}

//----------------------------------private slots:
void Dialog_artikel::on_lineEdit_filter_lieferant_textChanged(const QString &arg1)
{
    ui->comboBox_lieferant->clear();
    for(uint i=1; i<=lieferanten.zeilenanzahl() ;i++)
    {
        if(text_rechts(lieferanten.zeile(i), " / ").toUpper().contains(arg1.toUpper()))
        {
            ui->comboBox_lieferant->addItem(lieferanten.zeile(i));
        }
    }
}

//----------------------------------
