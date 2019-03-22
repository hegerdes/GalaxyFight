#ifndef PLANET_H
#define PLANET_H

#include <QGraphicsItem>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a Planet ship as GraphicsItem
 * @author meversmeyer
 */
class GraphicsPlanetItem : public QGraphicsItem
{
//    Q_OBJECT
public:
    GraphicsPlanetItem(int id);

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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

    int type() const override { return ItemTypes::Planet;}

    int getID();

    void selected(bool flag = true);

    void setOwner(PlayerType newOwner);

    PlayerType getOwner();

private:
    PlayerType m_player;
    int m_id;
    bool m_selected;
};
}

#endif // PLANET_H
