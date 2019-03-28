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
    setMinimumSize(setting.value("Bildschirm/Breite").toInt(), setting.value("Bildschirm/Hoehe").toInt());
    connect(m_scenehandler, &Scene2dHandler::planetSelected, m_infobar, &Infobar::set_selected_planet);

}

Scene2D::~Scene2D()
{
    delete ui;
}
}
