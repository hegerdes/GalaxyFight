#ifndef PLANET_H
#define PLANET_H

#include <QGraphicsItem>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a Planet ship as GraphicsItem
 * @author lkreienbrink & meversmeyer
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
     * @param flag true: draws with the next update an hq icon on the planet
     */
    void setIsHQ(bool flag);

    /**
     * @brief getOwner the current planet owner
     * @return PlayerType
     */
    PlayerType getOwner();

private:
    ///Planet owner
    PlayerType m_player;
    ///Planet id
    int m_id;
    ///selection flag
    bool m_selected;
    ///HQ flag
    bool m_is_hq;
};
}

#endif // PLANET_H
