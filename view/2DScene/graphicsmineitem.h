#ifndef GRAPHICSITEMMINE_H
#define GRAPHICSITEMMINE_H

#include <QGraphicsItem>
#include <QPainter>
#include <QIcon>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a fighter ship as GraphicsItem
 * @author meversmeyer
 */
class GraphicsMineItem : public QGraphicsItem
{
public:

    int type() const override;

    /**
     * @brief boundingRect
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief paint
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

}

#endif // GRAPHICSITEMMINE_H
