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
}


void StartScreen::on_playBut_clicked()
{
    //sends Signals when "Spielen" was clicked
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    emit gotoLoadingScreen();
    // emit startClient();
    QtConcurrent::run(QThreadPool::globalInstance(), [&](){
       client_global.sendReadyT("name",4);
       std::cerr << "\t" << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
       MapFactory& b = MapFactory::getinstance();
       std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
       Map::Ptr a = b.getMap(setting.value("Dateipfade/Map").toString().toStdString());
       std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";

       std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
       auto game_inst = ManageGame::getinstance();
       std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
       bool end_loop = false;
       while( client_global.player_No == player_no::unassigned )
        {
            std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
            end_loop = client_global.wait_for_readData(500);
            std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
            std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
            client_global.sendReadyT("name",4);
			sleep(1);
		}

       client_global.wait_for_readData(-1);
//TODO USE ONLY IF SERVER IS RUNNUNG
       if(client_global.player_No == 0)
       {
           game_inst->initialize_player(PlanetChanges::PLAYER1,0);
       std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
       }
       else if(client_global.player_No == 1)
       {
           //game_inst->initialize_player(PlanetChanges::PLAYER2,1);
           game_inst->initialize_player(PlanetChanges::PLAYER2,a->getNumberOfPlanets()-1); //@ahaker
       std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
       }
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
