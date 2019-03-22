#include "graphicsplanetitem.h"
#include <QPainter>
#include <QIcon>
#include <QDebug>

namespace asteroids {

GraphicsPlanetItem::GraphicsPlanetItem(int id): m_id(id), m_selected(false)
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
    case PLAYER1:
        icon = QIcon("./models/planetGreen.svg").pixmap(50, 50);
        break;
    case PLAYER2:
        icon = QIcon("./models/planetRed.svg").pixmap(50, 50);
        break;
    default:
        icon = QIcon("./models/planetGrey.svg").pixmap(50, 50);
        break;
    }

    painter->drawPixmap(0, 0, 50, 50, icon);

    if(m_selected)
    {
//        QRectF rect = boundingRect();
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

PlayerType GraphicsPlanetItem::getOwner()
{
    return m_player;
}
}
