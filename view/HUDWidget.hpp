#ifndef __HUDWIDGET__
#define __HUDWIDGET__

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QImage>
#include <iostream>

#include "rendering/SpaceCraft.hpp"

namespace asteroids
{
class HUDWidget : public QWidget
{
    Q_OBJECT
    public:
        HUDWidget(QWidget* parent = nullptr);
        void setFirstPerson(bool b);
        void setSpacecraft(SpaceCraft::Ptr s){m_myShip = s;}
        void setEnemy(SpaceCraft::Ptr s){m_enemyShip = s;}
    protected:
        void paintEvent(QPaintEvent* event);
    private:
        /// Painter to be used for painting the HUD
        QPainter m_p;

        /// Cockpit for first person view
        QImage m_cockpit;

        bool m_firstPerson;

        SpaceCraft::Ptr m_myShip;

        SpaceCraft::Ptr m_enemyShip;
};
}

#endif