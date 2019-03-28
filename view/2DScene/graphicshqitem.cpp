#include "graphicshqitem.h"

namespace asteroids {

int GraphicsHqItem::type() const
{
    return ItemTypes::HQ;
}

QRectF GraphicsHqItem::boundingRect() const
{
    return QRectF(0, 0, 20.0, 20.0);
}

void GraphicsHqItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPixmap icon = QIcon("./models/hq.svg").pixmap(20, 20);

    painter->drawPixmap(0, 0, icon);
}

}
