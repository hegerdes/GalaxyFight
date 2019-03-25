#include "HUDWidget.hpp"

#include <string>

namespace asteroids
{
    HUDWidget::HUDWidget(QWidget* parent):QWidget(parent), m_cockpit("models/spaceship-cockpit-png-3.png"), m_firstPerson(false)
    {
    }

    void HUDWidget::paintEvent(QPaintEvent* event)
    {
        QRect leftHP;
        QRect rightHP;
        //Durchsichtige Rechtecke auf denen der Name der Spieler steht
        QRect textLeftHP;
        QRect textRightHP;
        //Rechteck für Geschwindigkeit des Spielers
        QRect velocityBar;
        QRect textVelocityBar;
        int crossHairW = width()/200;
        int crossHairH = height()/15-height()/30;
        QRect crossHair[4];
        QRect fullScreen;
        if(!m_firstPerson)
        {
            leftHP = QRect(0, 0, width()/2 * m_myShip->getHP()/10, height()/50);
            rightHP = QRect(width()/2 + (width()/2 - width()/2 * m_enemyShip->getHP()/10), 0, width()/2 * m_enemyShip->getHP()/10, height()/50);
            textLeftHP = QRect(0, 0, width()/2 * 1, height()/50);
            textRightHP =  QRect(width()/2 + (width()/2 - width()/2 * 1), 0, width()/2 * 1, height()/50);
        }
        else
        {
            fullScreen = QRect(0, 0, width(), height());
            leftHP = QRect(2.01*width()/9, height()*90.5/100, width()/6 * m_myShip->getHP()/10, height()/15);
            rightHP = QRect(5.5*width()/9 + (width()/6-width()/6 * m_enemyShip->getHP()/10), height()*90.5/100, width()/6 * m_enemyShip->getHP()/10, height()/15);
            velocityBar = QRect(3.75*width()/9, height()*90.5/100, width()/6 * m_myShip->getCurrentSpeed()/m_myShip->getMaxSpeed() , height()/15);
            
            textLeftHP = QRect(2.01*width()/9, height()*90.5/100, width()/6 * 1, height()/15);
            textRightHP = QRect(5.5*width()/9 + (width()/6-width()/6 * 1), height()*90.5/100, width()/6 * 1, height()/15);
            textVelocityBar = QRect(3.75*width()/9, height()*90.5/100, width()/6 , height()/15);
            
            m_p.begin(this);
            m_p.drawImage(fullScreen, m_cockpit);
            m_p.end();
        }
        crossHair[0] = QRect(width()/2 - crossHairH/2 - crossHairH, height()/2 - crossHairW/2, crossHairH, crossHairW);
        crossHair[1] = QRect(width()/2 - crossHairH/2 + crossHairH, height()/2 - crossHairW/2, crossHairH, crossHairW);
        crossHair[2] = QRect(width()/2 - crossHairW/2, height()/2 - crossHairH/2 + crossHairH, crossHairW, crossHairH);
        crossHair[3] = QRect(width()/2 - crossHairW/2, height()/2 - crossHairH/2 - crossHairH, crossHairW, crossHairH);
        m_p.begin(this);
        m_p.drawRect(leftHP);
        m_p.drawRect(rightHP);
        m_p.drawRect(velocityBar);
        m_p.fillRect(leftHP, QColor(0, 100, 0));
        m_p.fillRect(rightHP, QColor(155, 0, 0));
        m_p.fillRect(velocityBar, QColor(255,165,0));
        m_p.drawRect(textLeftHP);
        m_p.drawRect(textRightHP);
        m_p.drawRect(textVelocityBar);
        for(QRect r : crossHair)
        {
            m_p.drawRect(r);
            m_p.fillRect(r, QColor(0, 100, 0));
        }
        
        //Schreibt Text in die HP-Bars
        m_p.setPen(Qt::white);
        m_p.setFont(QFont("liberation",15,99));
        m_p.drawText(textLeftHP, Qt::AlignCenter,"Player 1");
        m_p.drawText(textRightHP, Qt::AlignCenter,"Player 2");
        int currentSpeedInt = m_myShip->getCurrentSpeed()/m_myShip->getMaxSpeed() * 100;
        string currentSpeedString = std::to_string(currentSpeedInt) + "%";

        m_p.drawText(textVelocityBar, Qt::AlignCenter, QString::fromStdString(currentSpeedString));
        
        m_p.end();    

    }

    void HUDWidget::setFirstPerson(bool b)
    {
        m_firstPerson = b;
    }
}