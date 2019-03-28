#include "loosingscreen.h"
#include "ui_loosingscreen.h"

loosingscreen::loosingscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loosingscreen)
{
    ui->setupUi(this);

    int x = setting.value("Bildschirm/Breite", "1920").toInt();
    int y = setting.value("Bildschirm/Hoehe", "1080").toInt();

    ui->label->setGeometry(x/2-x/3,y/4,x/1.5,y);
}

loosingscreen::~loosingscreen()
{
    delete ui;
}
