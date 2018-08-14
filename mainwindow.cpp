#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clear();
    widget_tableeditor  = NULL;
    widget_artikel      = NULL;
    widget_lieferanten  = NULL;
    widget_lager        = NULL;
    widget_projekte     = NULL;
    widget_backup       = NULL;
    widget_personal     = NULL;
    widget_matlist      = NULL;
    widget_bestellung   = NULL;

    this->setWindowState(Qt::WindowMaximized);
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

        dbglobal.setHostName(dbeigen.get_host());
        dbglobal.setDatabaseName(dbeigen.get_dbname());
        dbglobal.setUserName(dbeigen.get_user());
        dbglobal.setPassword(dbeigen.get_pwd());
        dbglobal = QSqlDatabase::addDatabase(dbeigen.get_driver(), "dbglobal");
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
            mb.setText(tr("Wurzelverzeichnis nicht bekannt!\nProgramm wird beendet."));
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
    mb.setText(tr(msg.toStdString().c_str()));
    mb.exec();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    int posx = 1;
    int posy = ui->menuBar->height() + ui->mainToolBar->height();
    int b = this->width();
    int h = this->height()-posy;

    if(widget_tableeditor != NULL)
    {
        widget_tableeditor->setGeometry(posx,posy, b, h);
    }else if(widget_artikel != NULL)
    {
        widget_artikel->setGeometry(posx,posy, b, h);
    }else if(widget_lieferanten != NULL)
    {
        widget_lieferanten->setGeometry(posx,posy, b, h);
    }else if(widget_lager != NULL)
    {
        widget_lager->setGeometry(posx,posy, b, h);
    }else if(widget_projekte != NULL)
    {
        widget_projekte->setGeometry(posx,posy, b, h);
    }else if(widget_backup != NULL)
    {
        widget_backup->setGeometry(posx,posy, b, h);
    }else if(widget_personal != NULL)
    {
        widget_personal->setGeometry(posx,posy, b, h);
    }else if(widget_matlist != NULL)
    {
        widget_matlist->setGeometry(posx,posy, b, h);
    }else if(widget_bestellung != NULL)
    {
        widget_bestellung->setGeometry(posx,posy, b, h);
    }

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
                mb.setText(tr("Fehler beim Zugriff auf INI-Datei!\nProgramm wird beendet."));
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
        //dbeigen.set_param(ini.get_settings_db_eigen());
        //bis zum Neustart mit der alten Einstellung weiter machen!!
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
    }else
    {
        write_iniuser();
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
        change_windowtitle();

        //Rechte für Module setzen:
        ui_rechte_modul_artikel     (  u.modul_artikel()        );
        ui_rechte_modul_lieferanten (  u.modul_lieferanten()    );
        ui_rechte_modul_lager       (  u.modul_lager()          );
        ui_rechte_modul_projekte    (  u.modul_projekte()       );
        ui_rechte_modul_perrsonal   (  u.modul_personal()       );
        ui_rechte_modul_matlist     (  u.modul_matlist()        );
        ui_rechte_modul_bestellung  (  u.modul_bestellungen()   );
    }else
    {
        ui_rechte_nobody();
        change_windowtitle();
    }

    on_actionKeinModul_triggered();
}

void MainWindow::change_windowtitle()
{
    QString title;
    title += "CBrain";
    title += " / DB: ";
    title += dbeigen.get_dbname();
    title += " / User: ";
    title += u.get_current_user();
    //title += dbeigen.get_data_qstring(TABNAME_PERSONAL, PARAM_PERSONAL_VORNAME, u.get_current_user_id());
    title += " / ";
    title += currend_modul;

    this->setWindowTitle(title);
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
        if(data.get_text() != ini.get_settings_db_eigen().get_text())
        {
            ini.set_settings_db_eigen(data);
            write_inifile();
            QMessageBox mb;
            mb.setText(tr("Bitte starten Sie das Programm nun neu damit die Änderungen wirksam werden."));
            mb.exec();
        }
    }
}

void MainWindow::slot_write_inifile()
{
    write_inifile();
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
    on_actionModulPersonal_triggered();
    Dialog_user *d = new Dialog_user(this);
    connect(d, SIGNAL(signal_send_users(users)),        \
            this, SLOT(slot_get_users(users))           );
    d->set_users(u);
    d->exec();
    delete d;
}

void MainWindow::on_actionProgrammeigene_Datenbank_triggered()
{
    Dialog_settings_db *d = new Dialog_settings_db(this);
    connect(d, SIGNAL(signal_send_data(text_zeilenweise)),           \
            this, SLOT(slot_get_settings_db_eigen(text_zeilenweise)) );
    d->set_data(ini.get_settings_db_eigen());
    d->exec();
    delete d;
    //dbeigen.set_param(ini.get_settings_db_eigen());
    //bis zum Neustart mit der alten Einstellung weiter machen!!
}

//-----------------------------------------------Aktives Modul wechseln:
void MainWindow::on_actionKeinModul_triggered()
{
    change_modul("kein");
}
void MainWindow::on_actionTabelleneditor_triggered()
{
    change_modul("Tableeditor");
}
void MainWindow::on_actionModulArtikel_triggered()
{
    change_modul("Artikel");
}
void MainWindow::on_actionModulLieferanten_triggered()
{
    change_modul("Lieferanten");
}
void MainWindow::on_actionModulLager_triggered()
{
    change_modul("Lager");
}
void MainWindow::on_actionModulProjekte_triggered()
{
    change_modul("Projekte");
}
void MainWindow::on_actionModulBackup_triggered()
{
    change_modul("Backup");
}
void MainWindow::on_actionModulPersonal_triggered()
{
    change_modul("Personal");
}
void MainWindow::on_actionModulMaterialliste_triggered()
{
    change_modul("Materialliste");
}
void MainWindow::on_actionModulBestellungen_triggered()
{
    change_modul("Bestellungen");
}

void MainWindow::change_modul(QString modul)
{    
    if(widget_tableeditor != NULL)
    {
        delete widget_tableeditor;
        widget_tableeditor = NULL;
    }
    if(widget_artikel != NULL)
    {
        delete widget_artikel;
        widget_artikel = NULL;
    }
    if(widget_lieferanten != NULL)
    {
        delete widget_lieferanten;
        widget_lieferanten = NULL;
    }
    if(widget_lager != NULL)
    {
        delete widget_lager;
        widget_lager = NULL;
    }
    if(widget_projekte != NULL)
    {
        delete widget_projekte;
        widget_projekte = NULL;
    }
    if(widget_backup != NULL)
    {        
        disconnect(widget_backup, SIGNAL(signal_save_ini()),\
                this, SLOT(slot_write_inifile()));
        delete widget_backup;
        widget_backup = NULL;
    }
    if(widget_personal != NULL)
    {
        delete widget_personal;
        widget_personal = NULL;
    }
    if(widget_matlist != NULL)
    {
        delete widget_matlist;
        widget_matlist = NULL;
    }
    if(widget_bestellung != NULL)
    {
        delete widget_bestellung;
        widget_bestellung = NULL;
    }

    if(modul != "kein")
    {
        if(dbeigen.pingdb() == true)
        {
            int posx = 1;
            int posy = ui->menuBar->height() + ui->mainToolBar->height();
            int b = this->width();
            int h = this->height()-posy;

            if(modul == "Tableeditor")
            {
                currend_modul = "Tabelleneditor";
                widget_tableeditor = new Form_tableeditor(this);
                widget_tableeditor->setGeometry(posx, posy, b, h);
                widget_tableeditor->set_db(&dbeigen);
                widget_tableeditor->show();
            }else if(modul == "Artikel")
            {
                currend_modul = "Artikel";
                widget_artikel = new Form_artikel(this);
                widget_artikel->setGeometry(posx, posy, b, h);
                widget_artikel->set_user(u.get_current_user_id());
                widget_artikel->set_user(&u);
                widget_artikel->set_db(&dbeigen);
                widget_artikel->show();
            }else if(modul == "Lieferanten")
            {
                currend_modul = "Lieferanten";
                widget_lieferanten = new Form_lieferanten(this);
                widget_lieferanten->setGeometry(posx, posy, b, h);
                widget_lieferanten->set_user(u.get_current_user_id());
                widget_lieferanten->set_db(&dbeigen);
                widget_lieferanten->show();
            }else if(modul == "Lager")
            {
                currend_modul = "Lager";
                widget_lager = new Form_lager(this);
                widget_lager->setGeometry(posx, posy, b, h);
                widget_lager->set_user(u.get_current_user_id());
                widget_lager->set_db(&dbeigen);
                widget_lager->show();
            }else if(modul == "Projekte")
            {
                currend_modul = "Projekte";
                widget_projekte = new Form_projekte(this);
                widget_projekte->setGeometry(posx, posy, b, h);
                widget_projekte->set_user(u.get_current_user_id());
                widget_projekte->set_user(&u);
                widget_projekte->set_db(&dbeigen);
                widget_projekte->show();
            }else if(modul == "Backup")
            {
                currend_modul = "Backup";
                widget_backup = new Form_backup(this);
                widget_backup->setGeometry(posx, posy, b, h);
                widget_backup->set_db(&dbeigen);
                widget_backup->set_ini(&ini);
                connect(widget_backup, SIGNAL(signal_save_ini()),\
                        this, SLOT(slot_write_inifile()));
                widget_backup->show();
            }else if(modul == "Personal")
            {
                currend_modul = "Personal";
                widget_personal = new Form_personal(this);
                widget_personal->setGeometry(posx, posy, b, h);
                widget_personal->set_user(u.get_current_user_id());
                widget_personal->set_db(&dbeigen);
                widget_personal->show();
            }else if(modul == "Materialliste")
            {
                currend_modul = "Materialliste";
                widget_matlist = new Form_matlist(this);
                widget_matlist->setGeometry(posx, posy, b, h);
                widget_matlist->set_user(u.get_current_user_id());
                widget_matlist->set_db(&dbeigen);
                widget_matlist->show();
            }else if(modul == "Bestellungen")
            {
                currend_modul       = "Bestellungen";
                widget_bestellung = new Form_bestellung(this);
                widget_bestellung->setGeometry(posx, posy, b, h);
                widget_bestellung->set_user(u.get_current_user_id());
                widget_bestellung->set_db(&dbeigen);
                widget_bestellung->show();
            }else
            {
                currend_modul = "kein Modul geladen";
            }
        }else
        {
            QMessageBox mb;
            mb.setText(tr("Datenbank nicht erreichbar!\nModul wurden nicht geladen."));
            mb.exec();
        }
    }
    change_windowtitle();
}

//-----------------------------------------------UI den Rechten entsprechend anpassen:
void MainWindow::ui_rechte_nobody()
{
    ui->actionNetzwerkordner_aendern->setDisabled(true);
    ui->actionBenutzer_verwalten->setDisabled(true);
    ui->actionProgrammeigene_Datenbank->setDisabled(true);
    ui->actionTestfunktion->setDisabled(true);
    //ui->actionKeinModul->setDisabled(true);
    ui->actionTabelleneditor->setDisabled(true);
    ui->actionModulBackup->setDisabled(true);
    ui->actionTestfunktion->setVisible(false);

    ui->actionModulArtikel->setDisabled(true);
    ui->actionModulLieferanten->setDisabled(true);
    ui->actionModulLager->setDisabled(true);
    ui->actionModulProjekte->setDisabled(true);
    ui->actionModulPersonal->setDisabled(true);
    ui->actionModulMaterialliste->setDisabled(true);
    ui->actionModulBestellungen->setDisabled(true);
}

void MainWindow::ui_rechte_admin()
{
    ui->actionNetzwerkordner_aendern->setEnabled(true);
    ui->actionBenutzer_verwalten->setEnabled(true);
    ui->actionProgrammeigene_Datenbank->setEnabled(true);
    ui->actionTestfunktion->setEnabled(true);
    //ui->actionKeinModul->setEnabled(true);
    ui->actionTabelleneditor->setEnabled(true);
    ui->actionModulBackup->setEnabled(true);
    ui->actionTestfunktion->setVisible(true);

    //Die Rechte der nicht-Admin-Module werden separat gesteuert
}

void MainWindow::ui_rechte_modul_artikel(bool hat_rechte)
{
    if(hat_rechte == true)
    {
        ui->actionModulArtikel->setEnabled(true);
    }else
    {
        ui->actionModulArtikel->setDisabled(true);
    }
}
void MainWindow::ui_rechte_modul_lieferanten(bool hat_rechte)
{
    if(hat_rechte == true)
    {
        ui->actionModulLieferanten->setEnabled(true);
    }else
    {
        ui->actionModulLieferanten->setDisabled(true);
    }
}
void MainWindow::ui_rechte_modul_lager(bool hat_rechte)
{
    if(hat_rechte == true)
    {
        ui->actionModulLager->setEnabled(true);
    }else
    {
        ui->actionModulLager->setDisabled(true);
    }
}
void MainWindow::ui_rechte_modul_projekte(bool hat_rechte)
{
    if(hat_rechte == true)
    {
        ui->actionModulProjekte->setEnabled(true);
    }else
    {
        ui->actionModulProjekte->setDisabled(true);
    }
}
void MainWindow::ui_rechte_modul_perrsonal(bool hat_rechte)
{
    if(hat_rechte == true)
    {
        ui->actionModulPersonal->setEnabled(true);
    }else
    {
        ui->actionModulPersonal->setDisabled(true);
    }
}
void MainWindow::ui_rechte_modul_matlist(bool hat_rechte)
{
    if(hat_rechte == true)
    {
        ui->actionModulMaterialliste->setEnabled(true);
    }else
    {
        ui->actionModulMaterialliste->setDisabled(true);
    }
}
void MainWindow::ui_rechte_modul_bestellung(bool hat_rechte)
{
    if(hat_rechte == true)
    {
        ui->actionModulBestellungen->setEnabled(true);
    }else
    {
        ui->actionModulBestellungen->setDisabled(true);
    }
}

//-----------------------------------------------Testfunktion:
void MainWindow::on_actionTestfunktion_triggered()
{
    //-------------------------
    QMessageBox mb;
    mb.setText(tr("Die Testfunktion ist derzteit nicht in Nutzung."));
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







































