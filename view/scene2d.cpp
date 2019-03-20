#include "scene2d.h"
#include "ui_scene2d.h"

Scene2D::Scene2D(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene2D)
{
    ui->setupUi(this);
}

Scene2D::~Scene2D()
{
    delete ui;
}
