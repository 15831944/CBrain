#include "dialog_tableparam.h"
#include "ui_dialog_tableparam.h"

Dialog_tableparam::Dialog_tableparam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_tableparam)
{
    ui->setupUi(this);
}

Dialog_tableparam::~Dialog_tableparam()
{
    delete ui;
}

void Dialog_tableparam::on_pushButton_cancel_clicked()
{
    this->close();
}

void Dialog_tableparam::on_pushButton_ok_clicked()
{
    this->close();
    bool ispri;
    if(ui->checkBox_ispri->isChecked())
    {
        ispri = true;
    }else
    {
        ispri = false;
    }
    bool autoincrement;
    if(ui->checkBox_autoincrement->isChecked())
    {
        autoincrement = true;
    }else
    {
        autoincrement = false;
    }
    emit signal_send_dialog_data(ui->lineEdit_name->text(),\
                                 ui->comboBox_typ->currentText(),\
                                 ui->lineEdit_aditional->text(),\
                                 ispri,\
                                 autoincrement);
}

void Dialog_tableparam::set_windowTitle(QString title)
{
    this->setWindowTitle(title);
}

void Dialog_tableparam::setup_mysql()
{
    ui->comboBox_typ->clear();
    driver = "QMYSQL";

    //Numerisch
    ui->comboBox_typ->addItem("TINYINT");
    ui->comboBox_typ->addItem("SMALLINT");
    ui->comboBox_typ->addItem("MEDIUMINT");
    ui->comboBox_typ->addItem("INT");
    ui->comboBox_typ->addItem("BIGINT");
    ui->comboBox_typ->addItem("DECIMAL");
    ui->comboBox_typ->addItem("FLOAT");
    ui->comboBox_typ->addItem("DOUBLE");
    ui->comboBox_typ->addItem("BIT");
    ui->comboBox_typ->addItem("BOOLEAN");
    //Datum und Uhrzeit:
    ui->comboBox_typ->addItem("DATE");
    ui->comboBox_typ->addItem("DATETIME");
    ui->comboBox_typ->addItem("TIMESTAMP");
    ui->comboBox_typ->addItem("TIME");
    ui->comboBox_typ->addItem("YEAR");
    //Zeichenkette:
    ui->comboBox_typ->addItem("CHAR");
    ui->comboBox_typ->addItem("VARCHAR");
    ui->comboBox_typ->addItem("TINYTEXT");
    ui->comboBox_typ->addItem("TEXT");
    ui->comboBox_typ->addItem("MEDIUMTEXT");
    ui->comboBox_typ->addItem("LONGTEXT");
    ui->comboBox_typ->addItem("BINARY");
    ui->comboBox_typ->addItem("VARBINARY");
    ui->comboBox_typ->addItem("TINYBLOB");
    ui->comboBox_typ->addItem("BLOB");
    ui->comboBox_typ->addItem("MEDIUMBLOB");
    ui->comboBox_typ->addItem("LONGBLOB");
    ui->comboBox_typ->addItem("ENUM");
    ui->comboBox_typ->addItem("SET");
    //Räumlich:
    ui->comboBox_typ->addItem("GEOMETRY");
    ui->comboBox_typ->addItem("POINT");
    ui->comboBox_typ->addItem("LINESTRING");
    ui->comboBox_typ->addItem("POLYGON");
    ui->comboBox_typ->addItem("MULTIPOINT");
    ui->comboBox_typ->addItem("MULTILINESTRING");
    ui->comboBox_typ->addItem("MULTIPOLYGON");
    ui->comboBox_typ->addItem("GEOMETRYCOLLECTION");

}

void Dialog_tableparam::on_comboBox_typ_currentIndexChanged(const QString &arg1)
{
    QString msg;

    if(driver == "QMYSQL")
    {
        if(arg1 == "TINYINT")
        {
            msg += "1 Byte\n";
            msg += "Ganzzahlen\n";
            msg += "Bereich mit Vorzeichen: -128 bis 127\n";
            msg += "Bereich ohne Vorzeichen: 0 bis 255";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "SMALLINT")
        {
            msg += "2 Bytes\n";
            msg += "Ganzzahlen\n";
            msg += "Bereich mit Vorzeichen: -32.768 bis 32.767\n";
            msg += "Bereich ohne Vorzeichen: 0 bis 65.535";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "MEDIUMINT")
        {
            msg += "3 Bytes\n";
            msg += "Ganzzahlen\n";
            msg += "Bereich mit Vorzeichen: -8.388.608 bis 8.388.607\n";
            msg += "Bereich ohne Vorzeichen: 0 bis 16.277.215";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "INT")
        {
            msg += "4 Bytes\n";
            msg += "Ganzzahlen\n";
            msg += "Bereich mit Vorzeichen: -2.147.483.648 bis 2.147.483.647\n";
            msg += "Bereich ohne Vorzeichen: 0 bis 4.294.967.295";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "BIGINT")
        {
            msg += "8 Bytes\n";
            msg += "Ganzzahlen\n";
            msg += "Bereich mit Vorzeichen: -9.223.372.036.854.775.808 bis 9.223.372.036.854.775.807\n";
            msg += "Bereich ohne Vorzeichen: 0 bis 18.446.744.073.709.551.615";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "DECIMAL")
        {
            msg += "Fliesskommazahlen\n";
            msg += "Die maximale Anzahl an Ziffern (M) ist 65 (Standard: 10), die maximale Anzahl an Nachkommastellen (D) ist 30 (Standard: 0)";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "FLOAT")
        {
            msg += "4 Bytes\n";
            msg += "Fliesskommazahlen\n";
            msg += "Erlaubte Werte sind -3,402823466 x 10 38 bis -1,175494351 x 10 -38 , 0 und 1,175494351 x 10 -38 bis 3,402823466 x 10 38";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "DOUBLE")
        {
            msg += "8 Bytes\n";
            msg += "Fliesskommazahlen\n";
            msg += "Erlaubte Werte sind -1,7976931348623157 x 10 308 bis -2,2250738585072014 x 10 -308 , 0 und 2,2250738585072014 x 10 -308 bis 1,7976931348623157 x 10 308";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "BIT")
        {
            msg += "ca. (M+7)/8 Bytes\n";
            msg += "Ein Bitfeld-Typ (M), der M Bits pro Wert speichert (Standard ist 1, Maximum ist 64)";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "BOOLEAN")
        {
            msg += "1 Byte\n";
            msg += "Ein Synonym fuer TINYINT, ein Null- Wert wird als falsch angesehen, Nicht-Null- Werte werden als Wahr angesehen";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "DATE")
        {
            msg += "3 Bytes\n";
            msg += "Datum\n";
            msg += "Bereich ist 1000-01-01 bis 9999-12-31";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "DATETIME")
        {
            msg += "8 Bytes\n";
            msg += "Kombination aus Datum und Uhrzeit\n";
            msg += "Bereich ist 1000-01-01 00:00:00 bis 9999-12-31 23:59:59";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "TIMESTAMP")
        {
            msg += "4 Bytes\n";
            msg += "Zeitstempel\n";
            msg += "Bereich ist 1970-01-01 00:00:01 UTC bis 2038-01-09 03:14:07 UTC\n";
            msg += "Gespeichert als Anzahl der Sekunden seit Beginn der UNIX-Epoche (1970-01-01 00:00:00 UTC)";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "TIME")
        {
            msg += "3 Bytes\n";
            msg += "Uhrzeit\n";
            msg += "Bereich ist -838:59:59 bis 838:59:59";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "YEAR")
        {
            msg += "1 Byte\n";
            msg += "Jahr\n";
            msg += "Vier- (4, Standard) oder zweistelliges (2) Format\n";
            msg += "Erlaubte Werte sind 70 (1970) bis 69 (2069) oder 1901 bis 2155 und 0000";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "CHAR")
        {
            msg += "L Byte(s)\n";
            msg += "Zeichenkette mit fester Laenge (0 bis 255, Standard: 1) L\n";
            msg += "Rechts wird immer bis zur angegebenen Laenge mit Leerzeichen aufgefuellt";
            ui->label_aditional->setText("Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->setText("40");
        }else if(arg1 == "VARCHAR")
        {
            msg += "L+1 Byte(s)\n";
            msg += "Zeichenkette mit variabler Laenge (0 bis 65.535) L\n";
            msg += "Die tatsaechliche maximale Laenge haengt von der maximalen Anzahl an Zeilen ab";
            ui->label_aditional->setText("max Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->setText("40");
        }else if(arg1 == "TINYTEXT")
        {
            msg += "L+1 Byte(s)\n";
            msg += "TEXT-Spalte mit einer maximalen Laenge von 255 Zeichen\n";
            msg += "Gespeichert mit einem Ein-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("max Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->setText("40");
        }else if(arg1 == "TEXT")
        {
            msg += "L+2 Byte(s)\n";
            msg += "TEXT-Spalte mit einer maximalen Laenge von 65.535 Zeichen\n";
            msg += "Gespeichert mit einem Zwei-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("max Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->setText("40");
        }else if(arg1 == "MEDIUMTEXT")
        {
            msg += "L+3 Byte(s)\n";
            msg += "TEXT-Spalte mit einer maximalen Laenge von 16.777.215 Zeichen\n";
            msg += "Gespeichert mit einem Drei-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("max Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->setText("40");
        }else if(arg1 == "LONGTEXT")
        {
            msg += "L+4 Byte(s)\n";
            msg += "TEXT-Spalte mit einer maximalen Laenge von 4.294.967.295 oder 4 GiB Zeichen\n";
            msg += "Gespeichert mit einem Vier-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("max Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->setText("40");
        }else if(arg1 == "BINARY")
        {
            msg += "L Byte(s)\n";
            msg += "Aehnlich wie der CHAR-Typ, speichert aber binaere Byte-Zeichenketten anstelle von nicht-binaeren Buchstaben-Zeichenketten";
            ui->label_aditional->setText("max Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "VARBINARY")
        {
            msg += "L+1 Byte(s)\n";
            msg += "Aehnlich wie der VARCHAR-Typ, speichert aber binaere Byte-Zeichenketten anstelle von nicht-binaeren Buchstaben-Zeichenketten";
            ui->label_aditional->setText("max Zeichenanzahl:");
            ui->lineEdit_aditional->setEnabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "TINYBLOB")
        {
            msg += "L+1 Byte(s)\n";
            msg += "BLOB-Spalte mit einer maximalen Laenge L von 255 Zeichen\n";
            msg += "Gespeichert mit einem Ein-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);ui->lineEdit_aditional->clear();

        }else if(arg1 == "BLOB")
        {
            msg += "L+2 Byte(s)\n";
            msg += "BLOB-Spalte mit einer maximalen Laenge L von 65.535 Zeichen\n";
            msg += "Gespeichert mit einem Zwei-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "MEDIUMBLOB")
        {
            msg += "L+3 Byte(s)\n";
            msg += "BLOB-Spalte mit einer maximalen Laenge L von 16.777.215 Zeichen\n";
            msg += "Gespeichert mit einem Drei-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "LONGBLOB")
        {
            msg += "L+4 Byte(s)\n";
            msg += "BLOB-Spalte mit einer maximalen Laenge L von 4.294.967.295 oder 4 GiB Zeichen\n";
            msg += "Gespeichert mit einem Vier-Byte-Praefix, der die Laenge des Wertes in Bytes angibt";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "ENUM")
        {
            msg += "1-2 Byte(s)\n";
            msg += "Aufzaelung\n";
            msg += "Gewaehlt aus einer Liste von bis zu 65.535 Werten";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "SET")
        {
            msg += "1,2,3,4 oder 8 Byte(s)\n";
            msg += "String-Objekt\n";
            msg += "Ein einzelner Wert gewaehlt aus einem Satz von bis zu 64 Eintraegen";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "GEOMETRY")
        {
            msg += "Typ, der die Geometrie irgendeinen Typs speichern kann";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "POINT")
        {
            msg += "Punkt im 2-dimensionalen Raum";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "LINESTRING")
        {
            msg += "Kurve mit linearer Interpolation zwischen Punkten";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "POLYGON")
        {
            msg += "Vieleck (Polygon)";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "MULTIPOINT")
        {
            msg += "Punkte-Sammlung";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "MULTILINESTRING")
        {
            msg += "Kurven-Sammlung mit linearer Interpolation zwischen Punkten";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);ui->lineEdit_aditional->clear();

        }else if(arg1 == "MULTIPOLYGON")
        {
            msg += "Polygon-Sammlung";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else if(arg1 == "GEOMETRYCOLLECTION")
        {
            msg += "Sammlung von Geometrie-Objekten irgendeinen Typs";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }else
        {
            msg += "";
            ui->label_aditional->setText("...");
            ui->lineEdit_aditional->setDisabled(true);
            ui->lineEdit_aditional->clear();
        }

    }

    ui->label_info->setText(msg);
}










