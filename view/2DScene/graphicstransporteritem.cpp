#include "graphicstransporteritem.h"

#include <QIcon>
#include <QPixmap>
#include <QPainter>

namespace asteroids {

GraphicsTransporterItem::GraphicsTransporterItem(int id) : m_player(PLAYER1), m_id(id), m_selected(false)
{

}

QRectF GraphicsTransporterItem::boundingRect() const
{
    return QRectF(0, 0, 20, 20);
}

void GraphicsTransporterItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPixmap icon;

    //draw ship
    switch (m_player) {
    case PLAYER1:
        icon = QIcon("./models/transporter_player1.svg").pixmap(20, 20);
        break;
    case PLAYER2:
        icon = QIcon("./models/transporter_player1.svg").pixmap(20, 20);
        break;
    default:
        //show nothing
        return;
    }
    painter->drawPixmap(0, 0, 20, 20, icon);

    //draw ellipse if selected
    if(m_selected) {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(3);
        painter->setPen(pen);
        painter->drawEllipse(-3, -3, 26, 26);
    }
}

int GraphicsTransporterItem::getID()
{
    return m_id;
}

void GraphicsTransporterItem::selected(bool flag)
{
    m_selected = flag;
}

}
