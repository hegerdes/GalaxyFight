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
    ui->setupUi(this);
    QPixmap pic("models/start.jpg");
    ui->piclabel->setPixmap(pic);
}

StartScreen::~StartScreen()
{
    delete ui;
}
}

void asteroids::StartScreen::on_pushButton_3_clicked()
{
    emit closeProgramm();
}


void asteroids::StartScreen::on_pushButton_clicked()
{
    emit goToLoading();
}
