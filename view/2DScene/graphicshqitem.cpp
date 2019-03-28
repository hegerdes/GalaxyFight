#include "graphicshqitem.h"
#include "init_file.h"

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
    QPixmap icon = QIcon(setting.value("Dateipfade/HQ").toString()).pixmap(20, 20);

    painter->drawPixmap(0, 0, icon);
}

}
