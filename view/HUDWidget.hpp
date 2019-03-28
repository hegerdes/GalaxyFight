#ifndef __HUDWIDGET__
#define __HUDWIDGET__

#include <QWidget>
#include <QPainter>
#include <QRect>
#include <QImage>
#include <iostream>
#include <chrono>

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

        /**
         * @brief Sets when the last bullet was shot
         * 
         * @param lastBulletShot long that m_lastBulletShot is set to
         */ 
        void setBulletReady(long lastBulletShot);

    protected:

        /**
         * @brief Updates the HUDWidget whenever update() is called
         * 
         * @param event A paintevent
         */
        void paintEvent(QPaintEvent* event);
    private:

        /**
         * @brief Draws crosshair from 4 rectangles
         */
        void drawCrosshair();

        /**
         * @brief Draws health bars for both players
         */
        void drawHPBars();

        /**
         * @brief Draws the cockpit when in first person mode
         */
        void drawCockpit();

        /**
         * @brief Draws an arror pointing toward the enemy
         */
        void drawEnemyIndicator();

        /**
         * @brief Draws a bar indicating current velocity in relation to maximum velocity
         */
        void drawVelocityBar();

        /**
         * @brief Draws a bar indicating current distance to the enemy
         */
        void drawDistanceBar();

        /**
         * @brief Draws a circle that shows whether a bullet is ready
         */
        void drawShotReadyCircle();

        /**
         * @brief Draws victory/defeat screen and alerts
         */
        void drawScreenAlerts();

        ///Last Time an Alert was displayed on screen
        long m_lastAlert;

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

        ///time when the last Bullet was shot
        long m_lastBulletShot;

        ///Red Arrow 
        QImage m_enemyDirection;
};
}

#endif