#include "dialog_printbox.h"
#include "ui_dialog_printbox.h"

Dialog_printbox::Dialog_printbox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_printbox)
{
    ui->setupUi(this);
    ui->textEdit->setFontPointSize(18);//Textgröße
}

Dialog_printbox::~Dialog_printbox()
{
    delete ui;
}

void Dialog_printbox::setText(QString text)
{
    ui->textEdit->setText(text);
}

void Dialog_printbox::on_pushButton_print_clicked()
{
    QString filename;
    filename = QFileDialog::getSaveFileName(this,"Datei speichern",QString(),"PDF-Datei (*.pdf)");
    if(!filename.isEmpty())
    {
        if(QFileInfo(filename).suffix().isEmpty())
        {
            filename.append(".pdf");
        }

        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(filename);
        ui->textEdit->print(&printer);
    }
}
