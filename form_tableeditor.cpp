#include "form_tableeditor.h"
#include "ui_form_tableeditor.h"

Form_tableeditor::Form_tableeditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_tableeditor)
{
    ui->setupUi(this);
}

Form_tableeditor::~Form_tableeditor()
{
    delete ui;
}

void Form_tableeditor::clear()
{
    db = NULL;
    ui->listWidget_tables->clear();
}

void Form_tableeditor::resizeEvent(QResizeEvent *event)
{
    QRect rect_main =  this->geometry();
    int hoehe = rect_main.height();
    int breite = rect_main.width();

    ui->listWidget_tables->move(1,1);
    ui->listWidget_tables->setFixedSize(breite/5, hoehe-2);

    QWidget::resizeEvent(event);
}

void Form_tableeditor::set_db(cbrainbatabase *new_db)
{
    clear();
    db = new_db;

    text_zeilenweise tables = db->get_tables_tz();

    for(uint i=1; i<=tables.zeilenanzahl() ;i++)
    {
        ui->listWidget_tables->addItem(tables.zeile(i));
    }
}
