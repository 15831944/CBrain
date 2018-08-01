#include "dialog_settings_db.h"
#include "ui_dialog_settings_db.h"

Dialog_settings_db::Dialog_settings_db(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings_db)
{
    ui->setupUi(this);
    setup();
}

Dialog_settings_db::~Dialog_settings_db()
{
    delete ui;
}

void Dialog_settings_db::clear()
{
    ui->comboBox_driver->clear();
}

void Dialog_settings_db::setup()
{
    clear();

    ui->comboBox_driver->addItem("QDB2");       //IBM DB2 (version 7.1 and above)
    ui->comboBox_driver->addItem("QIBASE");     //Borland InterBase
    ui->comboBox_driver->addItem("QMYSQL");     //MySQL
    ui->comboBox_driver->addItem("QOCI");       //Oracle Call Interface Driver
    ui->comboBox_driver->addItem("QODBC");      //Open Database Connectivity (ODBC) - Microsoft SQL Server and other ODBC-compliant databases
    ui->comboBox_driver->addItem("QPSQL");      //PostgreSQL (versions 7.3 and above)
    ui->comboBox_driver->addItem("QSQLITE2");   //SQLite version 2
    ui->comboBox_driver->addItem("QSQLITE");    //SQLite version 3
    ui->comboBox_driver->addItem("QSYMSQL");    //SQLite version 3 for Symbian SQL Database
    ui->comboBox_driver->addItem("QTDS");       //Sybase Adaptive Server (Note: obsolete from Qt 4.7)
}

void Dialog_settings_db::set_data(text_zeilenweise data)
{
    ui->lineEdit_host->setText(data.zeile(1));
    ui->lineEdit_dbname->setText(data.zeile(2));

    QString driver = data.zeile(3);
    int index = ui->comboBox_driver->findText(driver);
    ui->comboBox_driver->setCurrentIndex(index);

    ui->lineEdit_username->setText(data.zeile(4));
    ui->lineEdit_userpwd->setText(data.zeile(5));
}

void Dialog_settings_db::on_pushButton_ok_clicked()
{
    QString msg;

    msg += ui->lineEdit_host->text();
    msg += "\n";
    msg += ui->lineEdit_dbname->text();
    msg += "\n";
    msg += ui->comboBox_driver->currentText();
    msg += "\n";
    msg += ui->lineEdit_username->text();
    msg += "\n";
    msg += ui->lineEdit_userpwd->text();

    text_zeilenweise tz;
    tz.set_text(msg);

    this->close();
    emit signal_send_data(tz);
}

void Dialog_settings_db::closeEvent(QCloseEvent *ce)
{
    QDialog::closeEvent(ce);
    emit signal_cancel();
}

void Dialog_settings_db::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_settings_db::on_pushButton_test_clicked()
{
    QSqlDatabase db = QSqlDatabase::addDatabase(ui->comboBox_driver->currentText(), "noname");
    db.setHostName(ui->lineEdit_host->text());
    db.setDatabaseName(ui->lineEdit_dbname->text());
    db.setUserName(ui->lineEdit_username->text());
    db.setPassword(ui->lineEdit_userpwd->text());

    bool ok = db.open();

    if(ok == true)
    {
        QMessageBox mb;
        mb.setText("Erfolgreich mit Datenbank verbunden!");
        mb.exec();
        db.close();
    }else
    {
        QMessageBox mb;
        mb.setText("Fehler:\n" + db.lastError().text());
        mb.exec();
    }

    db = QSqlDatabase();
    db.removeDatabase("noname");
}
