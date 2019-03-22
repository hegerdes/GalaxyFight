#ifndef GRAPHICSFIGHTERITEM_H
#define GRAPHICSFIGHTERITEM_H

#include <QGraphicsItem>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a fighter ship as GraphicsItem
 * @author lkreienbrink
 */
class GraphicsFighterItem : public QGraphicsItem
{
//    Q_OBJECT
public:
    GraphicsFighterItem(PlayerType type, int id);

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

    int type() const override { return ItemTypes::Fighter;}

    int getID();

    void selected(bool flag = true);

private:
    PlayerType m_player;
    int m_id;
    bool m_selected;
};
}
#endif // GRAPHICSFIGHTERITEM_H
