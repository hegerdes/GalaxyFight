#ifndef GRAPHICSITEMMINE_H
#define GRAPHICSITEMMINE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QIcon>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a mine as GraphicsItem
 * @author meversmeyer
 */
class GraphicsMineItem : public QGraphicsItem
{
public:

    /**
     * @brief type returns the GraphicsItem type
     * @return the custom GraphicsItem type representing a mine
     */
    int type() const override;

    /**
     * @brief boundingRect returns the GraphicsItem size
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief paint paints a mine svg pixmap
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

}

#endif // GRAPHICSITEMMINE_H
