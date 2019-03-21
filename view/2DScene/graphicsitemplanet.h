#ifndef PLANET_H
#define PLANET_H

#include <QGraphicsItem>
#include <QPainter>
#include <QPoint>
#include <QIcon>
#include <QSize>
#include "itemtypes.h"

namespace asteroids {

class GraphicsItemPlanet : public QGraphicsItem
{
public:

    enum Owner{UNASSIGN, PLAYER1, PLAYER2};

    GraphicsItemPlanet(int id);

    int type() const override;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    //void setPos(int x, int y);

    //QPoint getPos();

    void setOwner(Owner newOwner);

    Owner getOwner();

    void activate();

    void deactivate();

    int getId();

private:

    Owner m_owner;

    QPoint m_pos;

    bool isActive;

    int m_id;
};

}

#endif // PLANET_H
