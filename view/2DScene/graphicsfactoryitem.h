#ifndef GRAPHICSITEMFACTORY_H
#define GRAPHICSITEMFACTORY_H

#include <QGraphicsItem>
#include <QPainter>
#include <QIcon>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFactoryItem class represents a factory as GraphicsItem
 * @author meversmeyer
 */
class GraphicsFactoryItem : public QGraphicsItem
{
public:
    /**
     * @brief type returns the GraphicsItem type
     * @return the custom GraphicsItem type representing a factory
     */
    int type() const override;

    /**
     * @brief boundingRect returns the GraphicsItem size
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief paint paints a factory svg pixmap
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

}

#endif // GRAPHICSITEMFACTORY_H
