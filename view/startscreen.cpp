#include "startscreen.h"
#include "MainWindow.hpp"
#include <QFile>
#include <QPalette>
#include <QPixmap>
#include <QScreen>
#include <thread>
#include <future>
#include "global_socket.h"
#include <QtConcurrent>

namespace asteroids {

StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{
    // background-picture
    ui->setupUi(this);
    QPixmap pic("models/start.jpg");
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
}


void StartScreen::on_playBut_clicked()
{
    //sends Signals when "Spielen" was clicked
    emit gotoLoadingScreen();
    // emit startClient();
    client_global.sendReadyT("eins",4);
    QtConcurrent::run(QThreadPool::globalInstance(), [&](){
       MapFactory& b = MapFactory::getinstance();
       Map::Ptr a = b.getMap("models/01.map");

       auto game_inst = ManageGame::getinstance();

       client_global.wait_for_readData(-1);
//TODO USE ONLY IF SERVER IS RUNNUNG
//       if(client_global.player_No == 0)
//       {
//           game_inst->initialize_player(PlanetChanges::PLAYER1,0);
//       }
//       else if(client_global.player_No == 1)
//       {
//           game_inst->initialize_player(PlanetChanges::PLAYER2,a->getNumberOfPlanets());
//       }
        std::cerr << "player_No: " << client_global.player_No << ", id_other: " << client_global.id_other << "\n";
        emit goTo2D();
    });
}



void StartScreen::on_quitBut_clicked()
{
    //sends Signal when "Beenden" was clicked
    emit closeWindow();
}

void StartScreen::on_settingBut_clicked()
{
    //sends Signal when "Einstellungen" was clicked
    emit goto3DScene();
}

void StartScreen::setupConnections()
{
    //connect(this, &StartScreen::startClient , m_client, &Client::sendReadyT);
}
