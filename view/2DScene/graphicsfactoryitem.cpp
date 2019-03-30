#include "graphicsfactoryitem.h"
#include "settings/init_file.h"

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
    QPixmap icon = QIcon(setting.value("Dateipfade/Factory").toString()).pixmap(20, 20);

    painter->drawPixmap(0, 0, icon);
}

}
