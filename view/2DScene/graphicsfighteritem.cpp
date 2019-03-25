#include "graphicsfighteritem.h"
#include <QIcon>
#include <QPixmap>
#include <QPainter>

namespace asteroids {

GraphicsFighterItem::GraphicsFighterItem(PlayerType type, int id) : m_player(type), m_id(id), m_selected(false)
{

}

QRectF GraphicsFighterItem::boundingRect() const
{
    return QRectF(0, 0, 25, 25);
}

void GraphicsFighterItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPixmap icon;

    //draw ship
    switch (m_player) {
    case PlayerType::PLAYER1:
        icon = QIcon("./models/fighter_player1.svg").pixmap(25, 25);
        break;
    case PlayerType::PLAYER2:
        icon = QIcon("./models/fighter_player1.svg").pixmap(25, 25);
        break;
    default:
        //show nothing
        return;
    }
    painter->drawPixmap(0, 0, 25, 25, icon);
    //draw ellipse if selected
    if(m_selected) {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawEllipse(-3, -3, 30, 30);
    }
}

int GraphicsFighterItem::getID()
{
    return m_id;
}

void GraphicsFighterItem::selected(bool flag)
{
    m_selected = flag;
}

}
