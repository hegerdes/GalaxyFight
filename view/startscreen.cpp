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
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    emit gotoLoadingScreen();
    // emit startClient();
    client_global.sendReadyT("name",4);
    QtConcurrent::run(QThreadPool::globalInstance(), [&](){
		std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
		MapFactory& b = MapFactory::getinstance();
		Map::Ptr a = b.getMap("models/01.map");

		auto game_inst = ManageGame::getinstance();
		while(!client_global.wait_for_readData(500))
		{
			client_global.sendReadyT("name",4);
			sleep(1);
			std::cerr << __LINE__ << "\n";
		}
		std::cerr << __LINE__ << ", player_No: " << client_global.player_No << ", id_other: " << client_global.id_other << "\n";
//TODO USE ONLY IF SERVER IS RUNNUNG
       std::cerr << __LINE__ << "\n";
       if(client_global.player_No == 0)
       {
           game_inst->initialize_player(PlanetChanges::PLAYER1,0);
       std::cerr << __LINE__ << "\n";
       }
       else if(client_global.player_No == 1)
       {
           game_inst->initialize_player(PlanetChanges::PLAYER2,a->getNumberOfPlanets());
       std::cerr << __LINE__ << "\n";
       }
       std::cerr << __LINE__ << "\n";
        emit goTo2D();
    });
       std::cerr << __LINE__ << "\n";
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
