#ifndef GRAPHICSFIGHTERITEM_H
#define GRAPHICSFIGHTERITEM_H

#include <QGraphicsItem>
#include "itemtypes.h"

namespace asteroids {
/**
 * @brief The GraphicsFighterItem class represents a fighter ship as GraphicsItem
 * @author lkreienbrink
 */
class GraphicsFighterItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)
public:
    GraphicsFighterItem(PlayerType type, int id);

    /**
     * @brief boundingRect returns the GraphicsItem size
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief paint paints a fighter svg pixmap and a circle if the fighter is selected
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

    /**
     * @brief type returns the GraphicsItem type
     * @return the custom GraphicsItem type representing a fighter
     */
    int type() const override { return ItemTypes::Fighter;}

    /**
     * @brief getID returns the id of the fighter
     * @return the fighter id
     */
    int getID();

    /**
     * @brief selected activates or deactivates the selection circle
     * @param flag true: fighter is selected, false: fighter is not selected
     */
    void selected(bool flag = true);

private:
    PlayerType m_player;
    int m_id;
    bool m_selected;
};
}
#endif // GRAPHICSFIGHTERITEM_H
