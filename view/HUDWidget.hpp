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
        /**
         * @brief Constructor for this class
         * 
         * @param parent The parent widget
         */
        HUDWidget(QWidget* parent = nullptr);

        /**
         * @brief Set first Person mode
         * 
         * @param b The next setting
         */
        void setFirstPerson(bool b);

        /**
         * @brief Sets own Spacecraft
         * 
         * @param s shared_ptr to the Spacecraft
         */
        void setSpacecraft(SpaceCraft::Ptr s){m_myShip = s;}
        
        /**
         * @brief Sets enemy Spacecraft
         * 
         * @param s shared_ptr to the Spacecraft
         */
        void setEnemy(SpaceCraft::Ptr s){m_enemyShip = s;}
    protected:
        void paintEvent(QPaintEvent* event);
    private:
        /// Painter to be used for painting the HUD
        QPainter m_p;

        /// Cockpit for first person view
        QImage m_cockpit;

        /// Saves whether first person view is active
        bool m_firstPerson;

        /// Pointer to own ship
        SpaceCraft::Ptr m_myShip;

        /// Pointer to enemy ship
        SpaceCraft::Ptr m_enemyShip;
};
}

#endif