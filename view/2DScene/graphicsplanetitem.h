#ifndef PLANET_H
#define PLANET_H

#include <QGraphicsItem>
#include <QPainter>
#include <QIcon>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a fighter ship as GraphicsItem
 * @author meversmeyer
 */
class GraphicPlanetItem : public QGraphicsItem
{
public:

    GraphicPlanetItem(PlayerType type, int id);

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
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setOwner(PlayerType newOwner);

    PlayerType getOwner();

    void selected(bool flag);

    int getId();

private:

    PlayerType m_player;

    int m_id;

    bool m_selected;

};

}

#endif // PLANET_H
