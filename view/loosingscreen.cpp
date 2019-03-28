#include "loosingscreen.h"
#include "ui_loosingscreen.h"

loosingscreen::loosingscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loosingscreen)
{
    ui->setupUi(this);
}

loosingscreen::~loosingscreen()
{
    delete ui;
}
