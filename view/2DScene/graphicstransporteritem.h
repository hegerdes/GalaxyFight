#ifndef GRAPHICSTRANSPORTERITEM_H
#define GRAPHICSTRANSPORTERITEM_H

#include <QGraphicsItem>
#include "itemtypes.h"


namespace asteroids {
/**
 * @brief The GraphicsTransporterItem class
 * @author lkreienbrink
 */
class GraphicsTransporterItem : public QGraphicsItem
{
public:
    GraphicsTransporterItem(int id);


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

    int type() const override { return ItemTypes::Transporter;}

    int getID();

    void selected(bool flag = true);

private:
    PlayerType m_player;
    int m_id;
    bool m_selected;
};
}
#endif // GRAPHICSTRANSPORTERITEM_H
