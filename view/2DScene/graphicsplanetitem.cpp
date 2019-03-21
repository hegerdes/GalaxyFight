#include "graphicsplanetitem.h"

namespace asteroids {

GraphicsPlanetItem::GraphicsPlanetItem(int id): m_id(id), m_selected(false)
{

}


int GraphicsPlanetItem::type() const
{
    return ItemTypes::Planet;
}

QRectF GraphicsPlanetItem::boundingRect() const
{
    return QRectF(0, 0, 52.0, 52.0);
}

void GraphicsPlanetItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
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

void GraphicsPlanetItem::setOwner(PlayerType newOwner)
{
    m_player = newOwner;
}

PlayerType GraphicsPlanetItem::getOwner()
{
    return m_player;
}
void GraphicsPlanetItem::selected(bool flag)
{
    m_selected = flag;
}

int GraphicsPlanetItem::getId()
{
    return m_id;
}

}
