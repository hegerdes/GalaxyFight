#include "winningscreen.h"
#include "ui_winningscreen.h"

winningscreen::winningscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::winningscreen)
{
    ui->setupUi(this);
}

winningscreen::~winningscreen()
{
    delete ui;
}
