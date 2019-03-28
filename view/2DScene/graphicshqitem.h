#ifndef GRAPHICSHQITEM_H
#define GRAPHICSHQITEM_H

#include <QGraphicsItem>
#include <QPainter>
#include <QIcon>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsHqItem class represents a HQ as GraphicsItem
 * @author meversmeyer
 */
class GraphicsHqItem : public QGraphicsItem
{
public:

    /**
     * @brief type returns the GraphicsItem type
     * @return the custom GraphicsItem type representing a hq
     */
    int type() const override;

    /**
     * @brief boundingRect returns the GraphicsItem size
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief paint paints a hq svg pixmap
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

}

#endif // GRAPHICSHQITEM_H
