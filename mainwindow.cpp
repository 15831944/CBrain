#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clear();
    widget_tableeditor.setParent(this);
    on_actionKeinModul_triggered();

    //this->setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clear()
{
    isvalid = true;
    ini.clear();
    ui_rechte_nobody();
    modul_kein      = false;
    modul_tabedit   = false;
}

bool MainWindow::setup()
{
    isvalid = true;

    read_inifile();

    if(isvalid == true)
    {
        //Datei mit Benutzern suchen:
        read_iniuser();

        on_actionBenutzer_wechsen_triggered();

        widget_tableeditor.set_db(&dbeigen);    //widget Zeiger auf DB übergeben
    }

    return isvalid;
}

void MainWindow::on_actionNetzwerkordner_aendern_triggered()
{
    QString tmpdir;

    if(ini.get_rootdir().isEmpty())
    {
        tmpdir = "./";
    }else
    {
        tmpdir = ini.get_rootdir();
    }

    tmpdir = QFileDialog::getExistingDirectory(this, tr("Wurzelverzeichnis bestimmen"), tmpdir);
    if(!tmpdir.isEmpty())
    {
        ini.set_rootdir(tmpdir);
        write_inifile();
    }else
    {
        if(ini.get_rootdir().isEmpty())
        {
            QMessageBox mb;
            mb.setText("Wurzelverzeichnis nicht bekannt!\nProgramm wird beendet.");
            mb.exec();
            isvalid = false;
        }
    }
}

void MainWindow::on_actionInfo_triggered()
{
    QString msg;
    msg += "CBrain Version ";
    msg += VERSIONSNUMMER;
    msg += "\n";
    msg += "Autor: Oliver Schuft";
    msg += "\n";
    msg += "Quellcode: https://github.com/TischlerWilly/CBrain.git";
    //msg += "\n";

    QMessageBox mb;
    mb.setText(msg);
    mb.exec();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    int hoehe_menue =   ui->menuBar->geometry().height() +      \
                        ui->mainToolBar->geometry().height()    ;
    QRect rect_main =  this->geometry();
    int hoehe = rect_main.height()-hoehe_menue;
    int breite = rect_main.width();

    widget_tableeditor.move(0, hoehe_menue);
    widget_tableeditor.setFixedSize(breite, hoehe);

    QWidget::resizeEvent(event);
}

//-----------------------------------------------Inis:
void MainWindow::read_inifile()
{
    QString filename = INIFILE;
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ini.set_text( file.readAll()  );
        dbeigen.set_param(ini.get_settings_db_eigen());

    }else
    {
        on_actionNetzwerkordner_aendern_triggered();

        if(isvalid == true)
        {
            //Inifile anlegen:
            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                file.write(ini.get_rootdir().toUtf8());
                file.close();
            }else
            {
                QMessageBox mb;
                mb.setText("Fehler beim Zugriff auf INI-Datei!\nProgramm wird beendet.");
                mb.exec();
                isvalid = false;
            }
        }
    }
}

void MainWindow::write_inifile()
{
    QString filename = INIFILE;
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(ini.get_text().toUtf8());
        file.close();
        dbeigen.set_param(ini.get_settings_db_eigen());
    }
}

void MainWindow::read_iniuser()
{
    QString filename;
    filename =  ini.get_rootdir();
    filename += QDir::separator();
    filename += INIUSER;
    QFile file(filename);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        u.set_users(file.readAll());
        file.close();
    }
}

void MainWindow::write_iniuser()
{
    QString filename;
    filename =  ini.get_rootdir();
    filename += QDir::separator();
    filename += INIUSER;
    QFile file(filename);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        file.write(u.get_users().toUtf8());
        file.close();
    }
}

//-----------------------------------------------Slots:
void MainWindow::slot_login(QString user, QString pwd)
{
    if(u.login(user, pwd) == true)
    {
        if(u.is_admin() == true)
        {
            ui_rechte_admin();
        }else
        {
            ui_rechte_nobody();
        }
        this->setWindowTitle("CBrain /" + user);
    }else
    {
        ui_rechte_nobody();
        this->setWindowTitle("CBrain / Nobody");
    }
}

void MainWindow::slot_get_users(users new_users)
{
    u = new_users;
    write_iniuser();
}

void MainWindow::slot_get_settings_db_eigen(text_zeilenweise data)
{
    if(ini.get_settings_db_eigen().get_text()  !=  data.get_text())
    {
        ini.set_settings_db_eigen(data);
        write_inifile();
    }
}

//-----------------------------------------------Dialoge und Widgets ansprechen:
void MainWindow::on_actionBenutzer_wechsen_triggered()
{
    //Anmeldedialog:
    Dialog_login *login = new Dialog_login;
    connect(login, SIGNAL(signal_ok(QString,QString)),       \
            this, SLOT(slot_login(QString,QString))     );
    login->exec();
    delete login;
}

void MainWindow::on_actionBenutzer_verwalten_triggered()
{
    Dialog_user *d = new Dialog_user;
    connect(d, SIGNAL(signal_send_users(users)),        \
            this, SLOT(slot_get_users(users))           );
    d->set_users(u);
    d->exec();
    delete d;
}

void MainWindow::on_actionProgrammeigene_Datenbank_triggered()
{
    Dialog_settings_db *d = new Dialog_settings_db;
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),           \
            this, SLOT(slot_get_settings_db_eigen(text_zeilenweise)) );
    d->set_data(ini.get_settings_db_eigen());
    d->exec();
    delete d;
    dbeigen.set_param(ini.get_settings_db_eigen());
}

//-----------------------------------------------Aktives Modul wechseln:
void MainWindow::on_actionKeinModul_triggered()
{
    change_modul("kein");
}

void MainWindow::on_actionTabelleneditor_triggered()
{
    change_modul("tableeditor");
}

void MainWindow::change_modul(QString modul)
{
    if(modul == "tableeditor")
    {
        if(modul_tabedit == false)
        {
            modul_kein      = false;
            modul_tabedit   = true;


            widget_tableeditor.show();
        }
    }else
    {
        if(modul_kein == false)
        {
            modul_kein      = true;
            modul_tabedit   = false;

            widget_tableeditor.hide();
        }
    }

}

//-----------------------------------------------UI den Rechten entsprechend anpassen:
void MainWindow::ui_rechte_nobody()
{
    ui->actionNetzwerkordner_aendern->setDisabled(true);
    ui->actionBenutzer_verwalten->setDisabled(true);
    ui->actionProgrammeigene_Datenbank->setDisabled(true);
    ui->actionTestfunktion->setDisabled(true);
    ui->actionKeinModul->setDisabled(true);
    ui->actionTabelleneditor->setDisabled(true);
}

void MainWindow::ui_rechte_admin()
{
    ui->actionNetzwerkordner_aendern->setEnabled(true);
    ui->actionBenutzer_verwalten->setEnabled(true);
    ui->actionProgrammeigene_Datenbank->setEnabled(true);
    ui->actionTestfunktion->setEnabled(true);
    ui->actionKeinModul->setEnabled(true);
    ui->actionTabelleneditor->setEnabled(true);
}

//-----------------------------------------------Testfunktion:
void MainWindow::on_actionTestfunktion_triggered()
{
    //-------------------------
    QMessageBox mb;
    mb.setText("Die Testfunktion ist derzteit nicht in Nutzung.");
    mb.exec();
    //-------------------------
    /*
    if(dbeigen.open())
    {
        QMessageBox mb;
        mb.setText(dbeigen.get_tables_tz().get_text());
        mb.exec();
        dbeigen.close();
    }else
    {
        QMessageBox mb;
        mb.setText("nicht offen");
        mb.exec();
    }
    */
}

//-----------------------------------------------

























