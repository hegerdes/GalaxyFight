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
    QPixmap pic("models/start.jpg");
    ui->picLabel->setPixmap(pic);

    //loading-animation as a gif
    QMovie *movie = new QMovie("models/laden4.gif");
    ui->gifLabel->setMovie(movie);
    ui->gifLabel->setGeometry(x - screengeometry.width()/6,y - 50,screengeometry.width()/2,screengeometry.height()/2);
    movie->start();
    movie->setSpeed(75);


    ui->label->setGeometry(x - screengeometry.width()/3,y/4,screengeometry.width()/1.5,screengeometry.height()/4);


}

LoadingScreen::~LoadingScreen()
{
    delete ui;
}
}
