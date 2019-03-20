#include "loadingscreen.h"
#include "ui_loadingscreen.h"
#include <QMovie>
#include <QPixmap>
#include <QLabel>

LoadingScreen::LoadingScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadingScreen)
{
    ui->setupUi(this);

    QMovie *movie = new QMovie("models/loading.gif");
    ui->gifLabel->setMovie(movie);
    movie->start();




}

LoadingScreen::~LoadingScreen()
{
    delete ui;
}
