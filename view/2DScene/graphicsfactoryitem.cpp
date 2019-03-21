#include "graphicsfactoryitem.h"

namespace asteroids {

int GraphicsFactoryItem::type() const
{
    return ItemTypes::Factory;
}

QRectF GraphicsFactoryItem::boundingRect() const
{
    return QRectF(0, 0, 20.0, 20.0);
}

void GraphicsFactoryItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPixmap icon = QIcon("/models/factory.svg").pixmap(20, 20);

    painter->drawPixmap(0, 0, icon);
}

}
