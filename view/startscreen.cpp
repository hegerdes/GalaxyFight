#include "startscreen.h"
#include "MainWindow.hpp"
#include <QFile>
#include <QPalette>
#include <QPixmap>

namespace asteroids {

StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{
    // background-picture
    ui->setupUi(this);
    QPixmap pic("models/start.jpg");
    ui->piclabel->setPixmap(pic);
}

StartScreen::~StartScreen()
{
    delete ui;
}
}


void asteroids::StartScreen::on_joinBut_clicked()
{
    //sends Signals when "Spiel beitreten" was clicked
    emit goToLoading();
    emit startClient();
}


void asteroids::StartScreen::on_createBut_clicked()
{
    //sends Signals when "Spiel erstellen" was clicked
    emit goToLoading();
    emit startServer();
}

void asteroids::StartScreen::on_quitBut_clicked()
{
    //sends Signal when "Beenden" was clicked
    emit closeProgramm();
}

void asteroids::StartScreen::on_settingBut_clicked()
{
    //sends Signal when "Einstellungen" was clicked
    emit goToSetting();
}
