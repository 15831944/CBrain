#include "form_tableeditor.h"
#include "ui_form_tableeditor.h"


Form_tableeditor::Form_tableeditor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_tableeditor)
{
    ui->setupUi(this);
    model = new QSqlQueryModel();
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

    hoehe = hoehe - ui->label_tables->height();

    //Spalte 1 Tabellen-Namen:
    //--label:
    ui->label_tables->move(1,1);
    ui->label_tables->setFixedWidth(breite/5);
    //--listWidget
    ui->listWidget_tables->move( 1 ,                                                \
                                 1 + ui->label_tables->geometry().height() + 1      );
    ui->listWidget_tables->setFixedSize(breite/5, \
                                        hoehe - \
                                        ui->pushButton_table_new->height() - \
                                        ui->lineEdit_table_seach->height() - \
                                        7);
    //--Suchleiste:
    ui->lineEdit_table_seach->move( 1 ,\
                                    1 +\
                                    ui->label_tables->height() +\
                                    1 + \
                                    ui->listWidget_tables->height() + \
                                    1);
    ui->lineEdit_table_seach->setFixedWidth(ui->listWidget_tables->width());

    //--buttons:
    ui->pushButton_table_new->setFixedWidth(ui->listWidget_tables->width() / 3);
    ui->pushButton_table_del->setFixedWidth(ui->listWidget_tables->width() / 3 - 1);
    ui->pushButton_table_edit->setFixedWidth(ui->listWidget_tables->width() / 3 - 1);
    ui->pushButton_table_new->move(1, \
                                   this->height() - ui->pushButton_table_new->height());
    ui->pushButton_table_del->move(1 + ui->pushButton_table_new->width() + 1, \
                                   this->height() - ui->pushButton_table_new->height());
    ui->pushButton_table_edit->move(1 + ui->pushButton_table_new->width()*2 + 2, \
                                   this->height() - ui->pushButton_table_new->height());


    //Spalte 2 Tabellen-Kopft:
    //--label:
    ui->label_tablehead->move(1+ui->listWidget_tables->width()+1 , 1 );
    ui->label_tablehead->setFixedWidth(breite/5);
    //--listWidget
    ui->listWidget_tablehead->move( 1+ui->listWidget_tables->width()+1 , \
                                    1 + ui->label_tables->height() + 1   );
    ui->listWidget_tablehead->setFixedSize(breite/5, \
                                           hoehe - ui->pushButton_table_new->height() - 5);
    //--buttons:
    ui->pushButton_param_new->setFixedWidth(ui->listWidget_tables->width() / 3);
    ui->pushButton_param_del->setFixedWidth(ui->listWidget_tables->width() / 3 - 1);
    ui->pushButton_param_edit->setFixedWidth(ui->listWidget_tables->width() / 3 - 1);
    ui->pushButton_param_new->move(breite/5 +1 , \
                                   this->height() - ui->pushButton_table_new->height());
    ui->pushButton_param_del->move(breite/5 + 1 + ui->pushButton_param_new->width() + 1, \
                                   this->height() - ui->pushButton_table_new->height());
    ui->pushButton_param_edit->move(breite/5 + 1 + ui->pushButton_param_new->width()*2 + 2, \
                                   this->height() - ui->pushButton_table_new->height());



    //Spalte 3 :
    int labelbreite = ui->label_typlabel->width();
    if(labelbreite < ui->label_prilabel->width())
    {
        labelbreite = ui->label_prilabel->geometry().width();
    }
    if(labelbreite < ui->label_extralabel->width())
    {
        labelbreite = ui->label_extralabel->geometry().width();
    }

    ui->label_typlabel->setFixedWidth(labelbreite);
    ui->label_prilabel->setFixedWidth(labelbreite);
    ui->label_extralabel->setFixedWidth(labelbreite);

    int labelbreite2 =  breite -                                        \
                        1-                                              \
                        ui->listWidget_tables->width()-      \
                        1-                                              \
                        ui->listWidget_tablehead->width()-   \
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
                    ui->label_tables->width() +        \
                    1 +                                           \
                    ui->label_tablehead->width() +     \
                    1;
    int xposlabel2 = xposlabel + labelbreite + 1;

    ui->label_typlabel->move( xposlabel , 1 + ui->label_tables->height() + 1 );
    ui->label_typ->move     ( xposlabel2 , 1 + ui->label_tables->height() + 1 );

    ui->label_prilabel->move( xposlabel , (1 + ui->label_tables->height())*2 + 1 );
    ui->label_pri->move     ( xposlabel2 , (1 + ui->label_tables->height())*2 + 1 );

    ui->label_extralabel->move( xposlabel , (1 + ui->label_tables->height())*3 + 1 );
    ui->label_extra->move     ( xposlabel2 , (1 + ui->label_tables->height())*3 + 1 );

    ui->tableView->move(xposlabel, (1 + ui->label_tables->height())*4 + 1);
    ui->tableView->setFixedWidth(labelbreite + labelbreite2 -1);
    int tablehight = hoehe - ui->tableView->pos().ry()+ui->label_tables->height() -3 \
                     - 1 - ui->pushButton_value_edit->height();
    if(tablehight > 0)
    {
        ui->tableView->setFixedHeight(tablehight);
    }
    ui->pushButton_value_edit->move(xposlabel, ui->pushButton_param_edit->pos().y());
    ui->pushButton_value_edit->setFixedWidth(ui->tableView->width());

    QWidget::resizeEvent(event);
}

void Form_tableeditor::set_db(cbrainbatabase *new_db)
{    
    clear();
    dbeigen = new_db;

    update_tablnames();
}

void Form_tableeditor::on_listWidget_tables_currentRowChanged()
{
    //-------------------------------------------
    ui->listWidget_tablehead->clear();
    text_zeilenweise params = dbeigen->get_param_tz(ui->listWidget_tables->currentItem()->text());

    for(uint i=1; i<=params.zeilenanzahl() ;i++)
    {
        ui->listWidget_tablehead->addItem(params.zeile(i));
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
    text_zeilenweise types = dbeigen->get_param_type_tz(ui->listWidget_tables->currentItem()->text());
    if(currentRow != -1)//ist -1 wenn noch nichts gewählt wurde
    {
         ui->label_typ->setText(types.zeile(currentRow+1));
    }else
    {
        ui->label_typ->setText("...");
    }

    text_zeilenweise pri = dbeigen->get_param_primkey_tz(ui->listWidget_tables->currentItem()->text());
    if(currentRow != -1)//ist -1 wenn noch nichts gewählt wurde
    {
        if(pri.zeile(currentRow+1).contains("PRI"))
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

    text_zeilenweise extra = dbeigen->get_param_extra_tz(ui->listWidget_tables->currentItem()->text());
    if(currentRow != -1)//ist -1 wenn noch nichts gewählt wurde
    {
         ui->label_extra->setText(extra.zeile(currentRow+1));
    }else
    {
        ui->label_extra->setText("...");
    }
}

void Form_tableeditor::update_tablnames()
{
    ui->listWidget_tables->clear();
    text_zeilenweise tables = dbeigen->get_tables_tz();

    for(uint i=1; i<=tables.zeilenanzahl() ;i++)
    {
        QString name = tables.zeile(i);
        if(ui->lineEdit_table_seach->text().isEmpty())
        {
            ui->listWidget_tables->addItem(name);
        }else
        {
            if(name.toUpper().contains(  ui->lineEdit_table_seach->text().toUpper()  ))
            {
                ui->listWidget_tables->addItem(name);
            }
        }
    }
}

void Form_tableeditor::on_lineEdit_table_seach_textChanged()
{
    update_tablnames();
}
//----------------------------------Buttons:
void Form_tableeditor::on_pushButton_table_new_clicked()
{
    Dialog_text_input *d = new Dialog_text_input;
    d->setup("neue Tabelle anlegen","Bitte geben Sie den Namen der neuen Tabelle ein:");
    connect(d, SIGNAL(signal_userinput(QString)),           \
                      this, SLOT(slot_new_table(QString))   );
    d->exec();
    delete d;
}

void Form_tableeditor::on_pushButton_table_del_clicked()
{
    if(ui->listWidget_tables->currentRow() >= 0)
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
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte waelen Sie zuerst eine Tabelle aus!");
        mb.exec();
    }
}

void Form_tableeditor::on_pushButton_table_edit_clicked()
{
    if(ui->listWidget_tables->currentRow() >= 0)
    {
        Dialog_text_input *d = new Dialog_text_input;
        d->setup("Tabelle umbenennen","Bitte geben Sie den neuen Namen der Tabelle ein:");
        d->set_default_input(ui->listWidget_tables->currentItem()->text());
        connect(d, SIGNAL(signal_userinput(QString)),           \
                          this, SLOT(slot_rename_table(QString))   );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte waelen Sie zuerst eine Tabelle aus!");
        mb.exec();
    }
}

void Form_tableeditor::on_pushButton_param_new_clicked()
{
    if(ui->listWidget_tables->currentRow() >= 0)
    {
        Dialog_tableparam *d = new Dialog_tableparam;
        d->setWindowTitle("neuen Parameter anlegen");
        d->setup_mysql();   //derzeit wird nur mysql gebraucht, später evtl. Differenzierung
        connect(d, SIGNAL(signal_send_dialog_data(QString,QString,QString,bool,bool,bool,bool,QString)),  \
                this, SLOT(slot_new_param(QString,QString,QString,bool,bool,bool,bool,QString))           );
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte waelen Sie zuerst eine Tabelle aus!");
        mb.exec();
    }
}

void Form_tableeditor::on_pushButton_param_del_clicked()
{
    if(ui->listWidget_tables->currentRow() >= 0)
    {
        if(ui->listWidget_tablehead->currentRow() >= 0)
        {
            Dialog_yes_no *d = new Dialog_yes_no;
            QString msg;
            msg += "Wollen Sie diesen Parameter \"";
            msg += ui->listWidget_tablehead->currentItem()->text();
            msg += "\" in der Tabelle wirklich unwiederruflich entfernen?";
            d->setup(msg);
            connect(d, SIGNAL(signal_yes()), this, SLOT(slot_delete_param()));
            d->exec();
            delete d;
        }else
        {
            QMessageBox mb;
            mb.setText("Bitte waelen Sie zuerst einen Parameter aus!");
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte waelen Sie zuerst eine Tabelle aus!");
        mb.exec();
    }
}

void Form_tableeditor::on_pushButton_param_edit_clicked()
{
    if(ui->listWidget_tables->currentRow() >= 0)
    {
        if(ui->listWidget_tablehead->currentRow() >= 0)
        {
            int currentRow = ui->listWidget_tablehead->currentRow();
            Dialog_tableparam *d = new Dialog_tableparam;

            d->setWindowTitle("Parameter bearbeiten");
            d->setup_mysql();   //derzeit wird nur mysql gebraucht, später evtl. Differenzierung
            d->set_paramname(ui->listWidget_tablehead->currentItem()->text());
            text_zeilenweise types = dbeigen->get_param_type_tz(ui->listWidget_tables->currentItem()->text());
            d->set_paramtyp(types.zeile(currentRow+1));
            text_zeilenweise pri = dbeigen->get_param_primkey_tz(ui->listWidget_tables->currentItem()->text());
            if(pri.zeile(currentRow+1).contains("PRI"))
            {
                d->set_pri(true);
            }else
            {
                d->set_pri(false);
            }
            text_zeilenweise extra = dbeigen->get_param_extra_tz(ui->listWidget_tables->currentItem()->text());
            if(extra.zeile(currentRow+1).contains("auto_increment"))
            {
                d->set_autoincrement(true);
            }else
            {
                d->set_autoincrement(false);
            }
            text_zeilenweise notnull = dbeigen->get_param_notnull_tz(ui->listWidget_tables->currentItem()->text());
            if(notnull.zeile(currentRow+1).contains("NO"))
            {
                d->set_notnull(true);
            }else
            {
                d->set_notnull(false);
            }
            text_zeilenweise defaultvalue = dbeigen->get_param_default_tz(ui->listWidget_tables->currentItem()->text());
            QString defval = defaultvalue.zeile(currentRow+1);
            if(defval == "---")
            {
                defval.clear();
            }
            d->set_default(defval);

            connect(d, SIGNAL(signal_send_dialog_data(QString,QString,QString,bool,bool,bool,bool,QString)),  \
                    this, SLOT(slot_edit_param(QString,QString,QString,bool,bool,bool,bool,QString))           );
            d->exec();
            delete d;
        }else
        {
            QMessageBox mb;
            mb.setText("Bitte waelen Sie zuerst einen Parameter aus!");
            mb.exec();
        }
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte waelen Sie zuerst eine Tabelle aus!");
        mb.exec();
    }
}

void Form_tableeditor::on_pushButton_value_edit_clicked()
{
    if(ui->listWidget_tables->count() > 0 && ui->listWidget_tables->currentRow() >= 0)
    {
        Dialog_tableeditor *d = new Dialog_tableeditor;
        d->set_db(dbeigen);
        d->set_table(ui->listWidget_tables->currentItem()->text());
        connect(d, SIGNAL(signal_ok()),\
                this, SLOT(slot_refresh_tableview()));
        d->exec();
        delete d;
    }else
    {
        QMessageBox mb;
        mb.setText("Bitte zuerst eine Tabelle waelen!");
        mb.exec();
    }
}
//----------------------------------slots:
void Form_tableeditor::slot_delete_table()
{
    if(dbeigen->table_del(ui->listWidget_tables->currentItem()->text()) == true)
    {
        if(ui->listWidget_tables->count() > 1)
        {
            delete ui->listWidget_tables->currentItem();
        }else
        {
            ui->listWidget_tables->clear();
        }
    }
}

void Form_tableeditor::slot_delete_param()
{
    if(dbeigen->param_del(ui->listWidget_tables->currentItem()->text(),\
                          ui->listWidget_tablehead->currentItem()->text())  == true)
    {
        if(ui->listWidget_tablehead->count() > 1)
        {
            delete ui->listWidget_tablehead->currentItem();
        }else
        {
            ui->listWidget_tablehead->clear();
        }
    }
    on_listWidget_tables_currentRowChanged();
}

void Form_tableeditor::slot_rename_table(QString tablename_new)
{
    if(dbeigen->table_rename(ui->listWidget_tables->currentItem()->text(), tablename_new) == true)
    {
        ui->listWidget_tables->currentItem()->setText(tablename_new);
        ui->listWidget_tables->sortItems();
    }
}

void Form_tableeditor::slot_new_table(QString tablename)
{
    if(dbeigen->table_new(tablename) == true)
    {
        ui->listWidget_tables->addItem(tablename);
        ui->listWidget_tables->sortItems();
    }
}

void Form_tableeditor::slot_new_param(QString name, QString typ, QString additional, \
                                      bool ispri, bool autoincrement, bool isunsigned,\
                                      bool notnull, QString defaultvalue)
{
    if(dbeigen->param_new( ui->listWidget_tables->currentItem()->text(),\
                           name,\
                           typ,\
                           additional,\
                           ispri,\
                           autoincrement,\
                           isunsigned,\
                           notnull,\
                           defaultvalue) == true
      )
    {
        ui->listWidget_tablehead->addItem(name);
        on_listWidget_tables_currentRowChanged();
    }
}

void Form_tableeditor::slot_edit_param(QString name, QString typ, QString additional, \
                                       bool ispri, bool autoincrement, bool isunsigned,\
                                       bool notnull, QString defaultvalue)
{
    if(dbeigen->param_edit(ui->listWidget_tables->currentItem()->text(),\
                           ui->listWidget_tablehead->currentItem()->text(),\
                           name,\
                           typ,\
                           additional,\
                           ispri,\
                           autoincrement,\
                           isunsigned,\
                           notnull,\
                           defaultvalue) == true
      )
    {
        ui->listWidget_tablehead->currentItem()->setText(name);
        on_listWidget_tables_currentRowChanged();
    }
}
void Form_tableeditor::slot_refresh_tableview()
{
    on_listWidget_tables_currentRowChanged();
}

//----------------------------------

























