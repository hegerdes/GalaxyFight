#ifndef GRAPHICSITEMFACTORY_H
#define GRAPHICSITEMFACTORY_H

#include <QGraphicsItem>
#include <QPainter>
#include <QIcon>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a fighter ship as GraphicsItem
 * @author meversmeyer
 */
class GraphicsFactoryItem : public QGraphicsItem
{
public:

    int type() const override;

    QRectF boundingRect() const override;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;
};

}

#endif // GRAPHICSITEMFACTORY_H
