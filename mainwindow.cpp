#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clear();
    widget_tableeditor.setParent(this);
    widget_artikel.setParent(this);
    widget_lieferanten.setParent(this);
    widget_lager.setParent(this);
    widget_projekte.setParent(this);
    widget_backup.setParent(this);
    widget_personal.setParent(this);
    widget_matlist.setParent(this);

    on_actionKeinModul_triggered();

    connect(&widget_backup, SIGNAL(signal_save_ini()),\
            this, SLOT(slot_write_inifile()));

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
    modul_kein      = false;
    modul_tabedit   = false;
    modul_artikel   = false;
    modul_lieferanten = false;
    modul_lager     = false;
    modul_backup    = false;
    modul_projekte  = false;
    modul_backup    = false;
    modul_personal  = false;
    modul_matlist   = false;
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

        widget_tableeditor.set_db(&dbeigen);    //widget Zeiger auf DB übergeben
        widget_artikel.set_db(&dbeigen);        //widget Zeiger auf DB übergeben
        widget_lieferanten.set_db(&dbeigen);    //widget Zeiger auf DB übergeben
        widget_lager.set_db(&dbeigen);          //widget Zeiger auf DB übergeben
        widget_projekte.set_db(&dbeigen);       //widget Zeiger auf DB übergeben
        widget_backup.set_db(&dbeigen);         //widget Zeiger auf DB übergeben
        widget_personal.set_db(&dbeigen);       //widget Zeiger auf DB übergeben
        widget_matlist.set_db(&dbeigen);        //widget Zeiger auf DB übergeben

        widget_backup.set_ini(&ini);
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

    widget_artikel.move(0, hoehe_menue);
    widget_artikel.setFixedSize(breite, hoehe);

    widget_lieferanten.move(0, hoehe_menue);
    widget_lieferanten.setFixedSize(breite, hoehe);

    widget_lager.move(0, hoehe_menue);
    widget_lager.setFixedSize(breite, hoehe);

    widget_projekte.move(0, hoehe_menue);
    widget_projekte.setFixedSize(breite, hoehe);

    widget_backup.move(0, hoehe_menue);
    widget_backup.setFixedSize(breite, hoehe);

    widget_personal.move(0, hoehe_menue);
    widget_personal.setFixedSize(breite, hoehe);

    widget_matlist.move(0, hoehe_menue);
    widget_matlist.setFixedSize(breite, hoehe);

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
        widget_artikel.set_user(u.get_current_user_id());
        widget_lieferanten.set_user(u.get_current_user_id());
        widget_lager.set_user(u.get_current_user_id());
        widget_projekte.set_user(u.get_current_user_id());
        widget_personal.set_user(u.get_current_user());
        widget_matlist.set_user(u.get_current_user_id());
        //Rechte für Module setzen:
        ui_rechte_modul_artikel(  u.modul_artikel()  );
        ui_rechte_modul_lieferanten(  u.modul_lieferanten()  );
        ui_rechte_modul_lager(  u.modul_lager()  );
        ui_rechte_modul_projekte(  u.modul_projekte()  );
        ui_rechte_modul_perrsonal(  u.modul_personal()  );
        ui_rechte_modul_matlist(  u.modul_matlist()  );
    }else
    {
        ui_rechte_nobody();
        change_windowtitle();
    }
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
            mb.setText("Bitte starten Sie das Programm nun neu damit die Aenderungen wirksam werden.");
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

void MainWindow::hide_all_moduls()
{
    modul_kein          = false;
    modul_tabedit       = false;
    modul_artikel       = false;
    modul_lieferanten   = false;
    modul_lager         = false;
    modul_projekte      = false;
    modul_backup        = false;
    modul_personal      = false;
    modul_matlist       = false;

    widget_tableeditor.hide();
    widget_lieferanten.hide();
    widget_lager.hide();
    widget_artikel.hide();
    widget_projekte.hide();
    widget_backup.hide();
    widget_personal.hide();
    widget_matlist.hide();
}
void MainWindow::change_modul(QString modul)
{
    if(modul == "Tableeditor")
    {
        if(modul_tabedit == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Tabelleneditor";
                widget_tableeditor.update_tablnames();
                modul_tabedit       = true;
                widget_tableeditor.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else if(modul == "Artikel")
    {
        if(modul_artikel == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Artikel";
                modul_artikel       = true;
                widget_artikel.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else if(modul == "Lieferanten")
    {
        if(modul_lieferanten == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Lieferanten";
                modul_lieferanten   = true;
                widget_lieferanten.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else if(modul == "Lager")
    {
        if(modul_lager == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Lager";
                modul_lager         = true;
                widget_lager.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else if(modul == "Projekte")
    {
        if(modul_projekte == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Projekte";
                modul_projekte      = true;
                widget_projekte.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else if(modul == "Backup")
    {
        if(modul_backup == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Backup";
                modul_backup        = true;
                widget_backup.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else if(modul == "Personal")
    {
        if(modul_personal == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Personal";
                modul_personal      = true;
                widget_personal.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else if(modul == "Materialliste")
    {
        if(modul_matlist == false)
        {
            if(dbeigen.pingdb() == true)
            {
                hide_all_moduls();
                currend_modul = "Materialliste";
                modul_matlist      = true;
                widget_matlist.show();
            }else
            {
                QMessageBox mb;
                mb.setText("Datenbank nicht erreichbar!\nModul wurden nicht geladen.");
                mb.exec();
            }
        }
    }else
    {
        if(modul_kein == false)
        {
            hide_all_moduls();
            currend_modul = "kein Modul geladen";
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





































