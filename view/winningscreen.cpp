#include "winningscreen.h"
#include "ui_winningscreen.h"

winningscreen::winningscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::winningscreen)
{
    ui->setupUi(this);

    int x = setting.value("Bildschirm/Breite", "1920").toInt();
    int y = setting.value("Bildschirm/Hoehe", "1080").toInt();

    ui->label->setGeometry(x/2 -x/3,y/4,x/1.5,y);
}

winningscreen::~winningscreen()
{
    delete ui;
}
