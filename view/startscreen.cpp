#include "startscreen.h"
#include "MainWindow.hpp"
#include <QFile>
#include <QPalette>
#include <QPixmap>
#include <QScreen>
#include <thread>
#include <future>
#include <QtConcurrent>
#include <QSettings>
#include <QString>

namespace asteroids {

StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{

    // background-picture
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

    ui->playBut->setGeometry(x - breite/2,y - hoehe,breite,hoehe);
    ui->settingBut->setGeometry(x - breite/2,y,breite,hoehe);
    ui->quitBut->setGeometry(x - breite/2,y + 3*hoehe,breite,hoehe);


    ui->label->setGeometry(x - screengeometry.width()/4,10,screengeometry.width()/2,screengeometry.height()/4);


}

StartScreen::~StartScreen()
{
    delete ui;
}


void StartScreen::on_playBut_clicked()
{

    auto game_inst = ManageGame::getinstance();
    game_inst->initialize_player(PlanetChanges::PLAYER1,0);

    emit goTo2D();
}



void StartScreen::on_quitBut_clicked()
{
    //sends Signal when "Beenden" was clicked
    emit closeWindow();
}

void StartScreen::on_settingBut_clicked()
{
    //sends Signal when "Einstellungen" was clicked
    emit goToSetting();
}

void StartScreen::setupConnections()
{
    //connect(this, &StartScreen::startClient , m_client, &Client::sendReadyT);
}

}
