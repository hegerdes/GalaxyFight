#include "graphicsplanetitem.h"
#include <QPainter>
#include <QIcon>
#include <QDebug>
#include "init_file.h"

namespace asteroids {

GraphicsPlanetItem::GraphicsPlanetItem(int id): m_id(id), m_selected(false), m_is_hq(false)
{

}

QRectF GraphicsPlanetItem::boundingRect() const
{
    return QRectF(0, 0, 55.0, 55.0);
}

void GraphicsPlanetItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPixmap icon;
    //draw ship
    switch (m_player) {
    case PlayerType::PLAYER1:
        icon = QIcon(setting.value("Dateipfade/Player1Planet").toString()).pixmap(50, 50);
        break;
    case PlayerType::PLAYER2:
        icon = QIcon(setting.value("Dateipfade/Player2Planet").toString()).pixmap(50, 50);
        break;
    default:
        icon = QIcon(setting.value("Dateipfade/NeutralPlanet").toString()).pixmap(50, 50);
        break;
    }

    painter->drawPixmap(0, 0, 50, 50, icon);

    if (m_is_hq) {
        icon = QIcon("./models/hq.svg").pixmap(20, 20);
        painter->drawPixmap(15, 25, icon);
    }

    if(m_selected)
    {
        QRectF rect(0, 0, 52, 52);

        QPen pen(Qt::blue, 3);
        painter->setPen(pen);
        painter->drawEllipse(rect);
    }
}

int GraphicsPlanetItem::getID()
{
    return m_id;
}

void GraphicsPlanetItem::selected(bool flag)
{
    m_selected = flag;
}

void GraphicsPlanetItem::setOwner(PlayerType newOwner)
{
    m_player = newOwner;
}

void GraphicsPlanetItem::setIsHQ(bool flag)
{
    m_is_hq = flag;
}

PlayerType GraphicsPlanetItem::getOwner()
{
    return m_player;
}
}
