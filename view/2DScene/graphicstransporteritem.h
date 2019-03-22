#ifndef GRAPHICSTRANSPORTERITEM_H
#define GRAPHICSTRANSPORTERITEM_H

#include <QGraphicsItem>
#include "itemtypes.h"


namespace asteroids {
/**
 * @brief The GraphicsTransporterItem class represents a transporter
 * @author lkreienbrink
 */
class GraphicsTransporterItem : public QGraphicsItem
{
public:
    GraphicsTransporterItem(int id);


    /**
     * @brief boundingRect the size of the graphicsItems
     * @return
     */
    QRectF boundingRect() const override;

    /**
     * @brief paint paints a transporter svg and a circle if the transporter is selected
     * @param painter
     * @param option
     * @param widget
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget) override;

    /**
     * @brief type returns the GraphicsItem typw
     * @return The custom GraphicsItem ItemType for the transporter
     */
    int type() const override { return ItemTypes::Transporter;}

    /**
     * @brief getID returns the transporter id
     * @return transporter id
     */
    int getID();

    /**
     * @brief selected activates or deactivates the selection.
     * @param flag true if item is selected otherwise false
     */
    void selected(bool flag = true);

private:
    PlayerType m_player;
    int m_id;
    bool m_selected;
};
}
#endif // GRAPHICSTRANSPORTERITEM_H
