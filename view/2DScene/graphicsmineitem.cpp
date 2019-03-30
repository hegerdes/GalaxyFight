#include "graphicsmineitem.h"
#include "settings/init_file.h"

namespace asteroids {

int GraphicsMineItem::type() const
{
    return ItemTypes::Mine;
}

QRectF GraphicsMineItem::boundingRect() const
{
    return QRectF(0, 0, 20.0, 20.0);
}

void GraphicsMineItem::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    QPixmap icon = QIcon(setting.value("Dateipfade/Mine").toString()).pixmap(20, 20);

    painter->drawPixmap(0, 0, icon);
}

}
