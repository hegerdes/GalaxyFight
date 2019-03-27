#include "settingwindow.h"
#include "ui_settingwindow.h"
#include <QPixmap>
#include <QScreen>

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
    int y = screengeometry.height()/2;
    int hoehe = screengeometry.height()/10;
    int breite = screengeometry.width()/6;

    ui->Hoehe->setGeometry(x-breite/2,y -hoehe,breite,hoehe);
    ui->Breite->setGeometry(x-breite/2,y -hoehe,breite,hoehe);
    ui->Hoehe->setGeometry(x-breite/2,y -hoehe,breite,hoehe);
    ui->Hoehe->setGeometry(x-breite/2,y -hoehe,breite,hoehe);
    ui->Hoehe->setGeometry(x-breite/2,y -hoehe,breite,hoehe);


}

settingwindow::~settingwindow()
{
    delete ui;
}
