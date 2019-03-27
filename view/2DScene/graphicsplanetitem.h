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

    /**
     * @brief getID
     * @return
     */
    int getID();

    /**
     * @brief selected
     * @param flag
     */
    void selected(bool flag = true);

    /**
     * @brief setOwner
     * @param newOwner
     */
    void setOwner(PlayerType newOwner);

    /**
     * @brief setIsHQ
     * @param flag
     */
    void setIsHQ(bool flag);

    PlayerType getOwner();

private:
    PlayerType m_player;
    int m_id;
    bool m_selected;
    bool m_is_hq;
};
}

#endif // PLANET_H
