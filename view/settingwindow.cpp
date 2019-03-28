#include "settingwindow.h"
#include "ui_settingwindow.h"
#include <QPixmap>
#include <QScreen>
#include <iostream>
#include <QSettings>
#include "init_file.h"

settingwindow::settingwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::settingwindow)
{
    ui->setupUi(this);
    QPixmap pic(setting.value("Dateipfade/Hintergrund", ".").toString());
    ui->piclabel->setPixmap(pic);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screengeometry = screen->geometry();
    this->resize(screengeometry.width(),screengeometry.height());

    int x = screengeometry.width()/2;
    int y = screengeometry.height()/10;
    int hoehe = screengeometry.height()/10;
    int breite = screengeometry.width()/6;

    ui->Hoehe->setGeometry(x-breite,y,breite,hoehe);
    ui->Breite->setGeometry(x-breite,2*y,breite,hoehe);
    ui->Port->setGeometry(x-breite,4*y,breite,hoehe);
    ui->IP->setGeometry(x-breite,5*y,breite,hoehe);
    //ui->MapAuswahl->setGeometry(x-breite,7*y,breite,hoehe);

    ui->lHoehe->setGeometry(x,y,breite, hoehe);
    ui->lBreite->setGeometry(x,2*y,breite, hoehe);
    ui->lPort->setGeometry(x,4*y,breite, hoehe);
    ui->lIP->setGeometry(x,5*y,breite, hoehe);
   // ui->Map1But->setGeometry(x,y,breite, hoehe);

    ui->BackButton->setGeometry(x-2*breite/2,8*y,2*breite, hoehe);




}

settingwindow::~settingwindow()
{
    delete ui;
}

void settingwindow::on_BackButton_clicked()
{
    QString hoehe = ui->lHoehe->text();
    QString breite = ui->lBreite->text();
    QString port = ui->lPort->text();
    QString ip = ui->lIP->text();

    if(!hoehe.isEmpty() )
    {
        setting.setValue("Bildschirm/Hoehe", hoehe);
    }

    if(!breite.isEmpty() )
    {
        setting.setValue("Bildschirm/Breite", breite);
    }

    if(!port.isEmpty() )
    {
        setting.setValue("Netzwerk/Port", port);
    }

    if(!ip.isEmpty() )
    {
        setting.setValue("Netzwerk/IP", ip);
    }

    emit goToStart();

}
