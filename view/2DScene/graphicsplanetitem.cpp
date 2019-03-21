#include "graphicplanetitem.h"

namespace asteroids {

GraphicPlanetItem::GraphicPlanetItem(PlayerType type, int id): m_player(type), m_id(id), m_selected(false)
{

}


int GraphicPlanetItem::type() const
{
    return ItemTypes::Planet;
}

QRectF GraphicPlanetItem::boundingRect() const
{
    return QRectF(0, 0, 52.0, 52.0);
}

void GraphicPlanetItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPixmap icon;

    if(m_player == UNASSIGNED)
    {
        icon = QIcon("/models/planetGrey.svg").pixmap(50, 50);

    }
    else if (m_player == PLAYER1)
    {
        icon = QIcon("/models/planetGreen.svg").pixmap(50, 50);
    }
    else if (m_player == PLAYER2)
    {
        icon = QIcon("/models/planetRed.svg").pixmap(50, 50);
    }

    painter->drawPixmap(0, 0, icon);

    if(m_selected)
    {
        QRectF rect = boundingRect();

        QPen pen(Qt::red, 3);
        painter->setPen(pen);
        painter->drawEllipse(rect);
    }
}

void GraphicPlanetItem::setOwner(PlayerType newOwner)
{
    m_player = newOwner;
}

PlayerType GraphicPlanetItem::getOwner()
{
    return m_player;
}
void GraphicPlanetItem::selected(bool flag)
{
    m_selected = flag;
}

int GraphicPlanetItem::getId()
{
    return m_id;
}

}
