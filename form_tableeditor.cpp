#include "form_tableeditor.h"
#include "ui_form_tableeditor.h"


Form_tableeditor::Form_tableeditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_tableeditor)
{
    ui->setupUi(this);
    this->model = new QSqlQueryModel();
}

Form_tableeditor::~Form_tableeditor()
{
    delete ui;
    delete model;
}

void Form_tableeditor::clear()
{
    dbeigen = NULL;
    ui->listWidget_tables->clear();
    ui->listWidget_tablehead->clear();
}

void Form_tableeditor::resizeEvent(QResizeEvent *event)
{
    QRect rect_main =  this->geometry();
    int hoehe = rect_main.height();
    int breite = rect_main.width();

    hoehe = hoehe - ui->label_tables->geometry().height();

    //Spalte 1 Tabellen-Namen:
    //--label:
    ui->label_tables->move(1,1);
    ui->label_tables->setFixedWidth(breite/5);
    //--listWidget
    ui->listWidget_tables->move( 1 ,                                                \
                                 1 + ui->label_tables->geometry().height() + 1      );
    ui->listWidget_tables->setFixedSize(breite/5, \
                                        hoehe - ui->pushButton_table_new->geometry().height() - 5);
    //--buttons:
    ui->pushButton_table_new->setFixedWidth(ui->listWidget_tables->geometry().width() / 2);
    ui->pushButton_table_del->setFixedWidth(ui->listWidget_tables->geometry().width() / 2 - 1);
    ui->pushButton_table_new->move(1, \
                                   this->geometry().height() - ui->pushButton_table_new->geometry().height());
    ui->pushButton_table_del->move(1 + ui->pushButton_table_new->geometry().width() + 1, \
                                   this->geometry().height() - ui->pushButton_table_new->geometry().height());


    //Spalte 2 Tabellen-Kopft:
    //--label:
    ui->label_tablehead->move(1+ui->listWidget_tables->geometry().width()+1 , 1 );
    ui->label_tablehead->setFixedWidth(breite/5);
    //--listWidget
    ui->listWidget_tablehead->move( 1+ui->listWidget_tables->geometry().width()+1 , \
                                    1 + ui->label_tables->geometry().height() + 1   );
    ui->listWidget_tablehead->setFixedSize(breite/5, \
                                           hoehe - ui->pushButton_table_new->geometry().height() - 5);
    //--buttons:
    ui->pushButton_param_new->setFixedWidth(ui->listWidget_tables->geometry().width() / 3);
    ui->pushButton_param_del->setFixedWidth(ui->listWidget_tables->geometry().width() / 3 - 1);
    ui->pushButton_param_edit->setFixedWidth(ui->listWidget_tables->geometry().width() / 3 - 1);
    ui->pushButton_param_new->move(breite/5 +1 , \
                                   this->geometry().height() - ui->pushButton_table_new->geometry().height());
    ui->pushButton_param_del->move(breite/5 + 1 + ui->pushButton_param_new->geometry().width() + 1, \
                                   this->geometry().height() - ui->pushButton_table_new->geometry().height());
    ui->pushButton_param_edit->move(breite/5 + 1 + ui->pushButton_param_new->geometry().width()*2 + 2, \
                                   this->geometry().height() - ui->pushButton_table_new->geometry().height());



    //Spalte 3 :
    int labelbreite = ui->label_typlabel->geometry().width();
    if(labelbreite < ui->label_prilabel->geometry().width())
    {
        labelbreite = ui->label_prilabel->geometry().width();
    }
    if(labelbreite < ui->label_extralabel->geometry().width())
    {
        labelbreite = ui->label_extralabel->geometry().width();
    }

    ui->label_typlabel->setFixedWidth(labelbreite);
    ui->label_prilabel->setFixedWidth(labelbreite);
    ui->label_extralabel->setFixedWidth(labelbreite);

    int labelbreite2 =  breite -                                        \
                        1-                                              \
                        ui->listWidget_tables->geometry().width()-      \
                        1-                                              \
                        ui->listWidget_tablehead->geometry().width()-   \
                        1-                                              \
                        labelbreite-                                    \
                        1;

    if(labelbreite2 < 10)
    {
        labelbreite2 = 10;
    }

    ui->label_typ->setFixedWidth(labelbreite2);
    ui->label_pri->setFixedWidth(labelbreite2);
    ui->label_extra->setFixedWidth(labelbreite2);

    int xposlabel = 1 +                                           \
                    ui->label_tables->geometry().width() +        \
                    1 +                                           \
                    ui->label_tablehead->geometry().width() +     \
                    1;
    int xposlabel2 = xposlabel + labelbreite + 1;

    ui->label_typlabel->move( xposlabel , 1 + ui->label_tables->geometry().height() + 1 );
    ui->label_typ->move     ( xposlabel2 , 1 + ui->label_tables->geometry().height() + 1 );

    ui->label_prilabel->move( xposlabel , (1 + ui->label_tables->geometry().height())*2 + 1 );
    ui->label_pri->move     ( xposlabel2 , (1 + ui->label_tables->geometry().height())*2 + 1 );

    ui->label_extralabel->move( xposlabel , (1 + ui->label_tables->geometry().height())*3 + 1 );
    ui->label_extra->move     ( xposlabel2 , (1 + ui->label_tables->geometry().height())*3 + 1 );

    ui->tableView->move(xposlabel, (1 + ui->label_tables->geometry().height())*4 + 1);
    ui->tableView->setFixedWidth(labelbreite + labelbreite2 -1);
    int tablehight = hoehe - ui->tableView->pos().ry()+ui->label_tables->geometry().height() -3;
    if(tablehight > 0)
    {
        ui->tableView->setFixedHeight(tablehight);
    }

    QWidget::resizeEvent(event);
}

void Form_tableeditor::set_db(cbrainbatabase *new_db)
{    
    clear();
    dbeigen = new_db;

    text_zeilenweise tables = dbeigen->get_tables_tz();

    for(uint i=1; i<=tables.zeilenanzahl() ;i++)
    {
        ui->listWidget_tables->addItem(tables.zeile(i));
    }
}

void Form_tableeditor::on_listWidget_tables_currentRowChanged()
{
    //-------------------------------------------
    ui->listWidget_tablehead->clear();
    text_zeilenweise tables = dbeigen->get_table_head(ui->listWidget_tables->currentItem()->text());

    for(uint i=1; i<=tables.zeilenanzahl() ;i++)
    {
        ui->listWidget_tablehead->addItem(tables.zeile(i));
    }
    //-------------------------------------------
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
            cmd += "SELECT * FROM ";
            cmd += ui->listWidget_tables->currentItem()->text();

            if(q.exec(cmd))
            {
                model->setQuery(q);
                ui->tableView->setModel(model);
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
            mb.setText("Fehler bei Datenbankverbindung!");
            mb.exec();
        }
    }
    //-------------------------------------------
}

void Form_tableeditor::on_listWidget_tablehead_currentRowChanged(int currentRow)
{
    text_zeilenweise types = dbeigen->get_table_head_type(ui->listWidget_tables->currentItem()->text());
    if(currentRow != -1)//ist -1 wenn noch nichts gewählt wurde
    {
         ui->label_typ->setText(types.zeile(currentRow+1));
    }else
    {
        ui->label_typ->setText("...");
    }

    text_zeilenweise pri = dbeigen->get_table_is_primary_key(ui->listWidget_tables->currentItem()->text());
    if(currentRow != -1)//ist -1 wenn noch nichts gewählt wurde
    {
        if(pri.zeile(currentRow+1) == "PRI")
        {
            ui->label_pri->setText("ja");
        }else
        {
            ui->label_pri->setText("nein");
        }
    }else
    {
        ui->label_pri->setText("...");
    }

    text_zeilenweise extra = dbeigen->get_table_value_extra(ui->listWidget_tables->currentItem()->text());
    if(currentRow != -1)//ist -1 wenn noch nichts gewählt wurde
    {
         ui->label_extra->setText(extra.zeile(currentRow+1));
    }else
    {
        ui->label_extra->setText("...");
    }
}

//----------------------------------Buttons:
void Form_tableeditor::on_pushButton_table_new_clicked()
{
    //noch Dialog vorher programmieren zum Eingeben des Nahmens

    QString name = "def";
    if(dbeigen->new_table(name) == true)
    {
        ui->listWidget_tables->addItem(name);
        ui->listWidget_tables->sortItems();
    }
}

void Form_tableeditor::on_pushButton_table_del_clicked()
{
    Dialog_yes_no *d = new Dialog_yes_no;
    QString msg;
    msg += "Wollen Sie die Tabelle \"";
    msg += ui->listWidget_tables->currentItem()->text();
    msg += "\" in der Datenbank wirklich unwiederruflich entfernen?";
    d->setup(msg);
    connect(d, SIGNAL(signal_yes()), this, SLOT(slot_delete_table()));
    d->exec();
    delete d;
}

//----------------------------------slots:
void Form_tableeditor::slot_delete_table()
{
    if(dbeigen->del_table(ui->listWidget_tables->currentItem()->text()) == true)
    {
        delete ui->listWidget_tables->currentItem();
    }
}

//----------------------------------












