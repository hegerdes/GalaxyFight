#include "graphicsItemPlanet.h"

GraphicsItemPlanet::GraphicsItemPlanet(int id)
{
    setFlag(ItemIsMovable);
    m_owner = UNASSIGN;
    m_pos = QPoint(0, 0);
    isActive = false;
    m_id = id;
}


int GraphicsItemPlanet::type() const
{
    return 0; //ItemTypes::Planet
}

QRectF GraphicsItemPlanet::boundingRect() const
{
    return QRectF(-55.0, -55.0, 110.0, 110.0);
}

void GraphicsItemPlanet::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPixmap icon;

    if(m_owner == UNASSIGN)
    {
        icon = QIcon("/models/planetGrey.svg").pixmap(QSize(100, 100));

    }
    else if (m_owner == PLAYER1)
    {
        icon = QIcon("/models/planetRed.svg").pixmap(QSize(100, 100));
    }
    else if (m_owner == PLAYER2)
    {
        icon = QIcon("/models/planetBlue.svg").pixmap(QSize(100, 100));
    }

    painter->drawPixmap(m_pos, icon);

    if(isActive)
    {
        QRectF rect = boundingRect();

        QPen pen(Qt::red, 3);
        painter->setPen(pen);
        painter->drawEllipse(rect);
    }
}

//void Planet::setPos(int x, int y)
//{
//    pos.setX(x);
//    pos.setY(y);
//}

//QPoint Planet::getPos()
//{
//    return pos;
//}

void GraphicsItemPlanet::setOwner(Owner newOwner)
{
    m_owner = newOwner;
}

GraphicsItemPlanet::Owner GraphicsItemPlanet::getOwner()
{
    return m_owner;
}

void GraphicsItemPlanet::activate()
{
    isActive = true;
}

void GraphicsItemPlanet::deactivate()
{
    isActive = false;
}

int GraphicsItemPlanet::getId()
{
    return m_id;
}
