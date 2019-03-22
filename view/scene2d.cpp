#include "scene2d.h"

#include <QSettings>

namespace asteroids {

Scene2D::Scene2D(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene2D)
{
    //Setup user interface
    ui->setupUi(this);

    m_graphicsview = ui->graphicsView;
    m_infobar = ui->infobar;

    m_scenehandler = new Scene2dHandler(this);

    //config scene
    m_graphicsview->setScene(m_scenehandler);
    m_graphicsview->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
    m_graphicsview->show();

    //config screen size
    QSettings settings;
    setMinimumSize(settings.value("minWidth", 1920).toInt(), settings.value("minHeight", 1080).toInt());

    connect(m_scenehandler, &Scene2dHandler::planetSelected, this, &Scene2D::planetSelected);
}

Scene2D::~Scene2D()
{
    delete ui;
}
}
