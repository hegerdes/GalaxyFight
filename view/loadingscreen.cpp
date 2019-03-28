#include "loadingscreen.h"
#include "ui_loadingscreen.h"
#include <QMovie>
#include <QPixmap>
#include <QScreen>

namespace asteroids {

LoadingScreen::LoadingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingScreen)
{
    ui->setupUi(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screengeometry = screen->geometry();
    this->resize(screengeometry.width(),screengeometry.height());

    int x = screengeometry.width()/2;
    int y = screengeometry.height()/2;


    //background-picture
    QPixmap pic(setting.value("Dateipfade/Hintergund").toString());
    ui->picLabel->setPixmap(pic);

    //loading-animation as a gif
    QMovie *movie = new QMovie(setting.value("Dateipfade/Laden").toString());
    ui->gifLabel->setMovie(movie);
    ui->gifLabel->setGeometry(x - screengeometry.width()/8,y -100,screengeometry.width()/4,screengeometry.height()/4 + 100);
    movie->setScaledSize(ui->gifLabel->size());
    movie->start();
    movie->setSpeed(75);


    ui->label->setGeometry(x - screengeometry.width()/3,y/4,screengeometry.width()/1.5,screengeometry.height()/4);


}

LoadingScreen::~LoadingScreen()
{
    delete ui;
}
}
