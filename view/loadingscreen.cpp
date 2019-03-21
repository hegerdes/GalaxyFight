#include "loadingscreen.h"
#include "ui_loadingscreen.h"
#include <QMovie>
#include <QPixmap>

LoadingScreen::LoadingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingScreen)
{
    ui->setupUi(this);


    //background-picture
    QPixmap pic("models/start.jpg");
    ui->picLabel->setPixmap(pic);

    //loading-animation as a gif
    QMovie *movie = new QMovie("models/laden4.gif");
    ui->gifLabel->setMovie(movie);
    movie->start();

}

LoadingScreen::~LoadingScreen()
{
    delete ui;
}
