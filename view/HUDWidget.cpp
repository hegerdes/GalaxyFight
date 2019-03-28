#include "HUDWidget.hpp"

#include <string>

namespace asteroids
{
    HUDWidget::HUDWidget(QWidget* parent):QWidget(parent), m_cockpit("models/spaceship-cockpit-png-3.png"), m_firstPerson(false), m_lastAlert(0), m_lastBulletShot(0),m_enemyDirection("models/red_arrow.png")
    {

    }

    void HUDWidget::paintEvent(QPaintEvent* event)
    {
        ///HP-Balken
        QRect leftHP;
        QRect rightHP;
        
        ///Durchsichtige Rechtecke auf denen der Name der Spieler steht
        QRect textLeftHP;
        QRect textRightHP;
        
        ///Rechteck für Geschwindigkeit des Spielers
        QRect velocityBar;
        QRect textVelocityBar;
        
        ///Rechteck für die Distanz zum Gegner
        QRect distanceBarLeft;
        QRect distanceBarRight;
        QRect textDistanceBar;

        int crossHairW = width()/200;
        int crossHairH = height()/15-height()/30;
        QRect crossHair[4];
        QRect fullScreen;

        ///Rechteck in dem Sieges/Niederlage/Alaramnachricht steht
        QRect onScreenMessage;

        ///Rechteck indem ein Kreis sitzt der anzeigt ob ein Schuß bereit ist
        QRect shotReadyRect;

        QTransform trans;
        
        ///Berechnet Distanz; diese wird später in der textDistanceBar ausgegeben
        Vector3f posMyShip = m_myShip->getPosition();
        Vector3f posEnemyShip = m_enemyShip->getPosition();
        float distance = sqrt(pow(posEnemyShip[0] - posMyShip[0],2) + pow(posEnemyShip[1] - posMyShip[1],2) + pow(posEnemyShip[2] - posMyShip[2],2));
        
        ///Distanzfunktion die zum Bewegen der Distanzanzeige benutzt wird
        float distanceFunction =1/(log(distance/100 +2));
        
        if(!m_firstPerson)
        {
            leftHP = QRect(0, 0, width()/2 * m_myShip->getHP()/10, height()/50);
            rightHP = QRect(width()/2 + (width()/2 - width()/2 * m_enemyShip->getHP()/10), 0, width()/2 * m_enemyShip->getHP()/10, height()/50);
            textLeftHP = QRect(0, 0, width()/2 * 1, height()/50);
            textRightHP =  QRect(width()/2 + (width()/2 - width()/2 * 1), 0, width()/2 * 1, height()/50);

            trans =QTransform (1, 0, 0, 1, width()/1.1, height()/6);
        }
        else
        {
            fullScreen = QRect(0, 0, width(), height());
            leftHP = QRect(2.01*width()/9, height()*90.5/100, width()/6 * m_myShip->getHP()/10, height()/15);
            rightHP = QRect(5.5*width()/9 + (width()/6-width()/6 * m_enemyShip->getHP()/10), height()*90.5/100, width()/6 * m_enemyShip->getHP()/10, height()/15);
            velocityBar = QRect(3.75*width()/9, height()*90.5/100, width()/6 * m_myShip->getCurrentSpeed()/m_myShip->getMaxSpeed() , height()/15);            
            
            ///Textbalken die über den Anzeigebalken liegen müssen, damit die Schrift nicht mit den Anzeigen mitskaliert bei Änderungen
            textLeftHP = QRect(2.01*width()/9, height()*90.5/100, width()/6 * 1, height()/15);
            textRightHP = QRect(5.5*width()/9 + (width()/6-width()/6 * 1), height()*90.5/100, width()/6 * 1, height()/15);
            textVelocityBar = QRect(3.75*width()/9, height()*90.5/100, width()/6 , height()/15);

            distanceBarRight = QRect(3.75*width()/9, height()*70/100, width()/12 * distanceFunction, height()/15);
            distanceBarLeft = QRect((3.75*width()/9)  + (width()/6 - width()/12 * distanceFunction), height()*70/100, width()/12 * distanceFunction, height()/15);
            
            textDistanceBar = QRect(3.75*width()/9, height()*70/100, width()/6 , height()/15);

            shotReadyRect =   QRect(3.1 * width()/9, height()*67/100, width()/18, height()/12);

            trans =QTransform (1, 0, 0, 1, width()/1.59, height()/1.39);
            trans.scale(0.45,0.45);
 
            m_p.begin(this);  
            m_p.drawImage(fullScreen, m_cockpit);
            m_p.end();
        }       

        crossHair[0] = QRect(width()/2 - crossHairH/2 - crossHairH, height()/2 - crossHairW/2, crossHairH, crossHairW);
        crossHair[1] = QRect(width()/2 - crossHairH/2 + crossHairH, height()/2 - crossHairW/2, crossHairH, crossHairW);
        crossHair[2] = QRect(width()/2 - crossHairW/2, height()/2 - crossHairH/2 + crossHairH, crossHairW, crossHairH);
        crossHair[3] = QRect(width()/2 - crossHairW/2, height()/2 - crossHairH/2 - crossHairH, crossHairW, crossHairH);
        
        Vector3f c = m_myShip->getZAxis();

        m_p.begin(this);
        
        ///Färbt die AnzeigeBars ein
        m_p.fillRect(leftHP, QColor(0, 100, 0));
        m_p.fillRect(rightHP, QColor(155, 0, 0));
        m_p.fillRect(velocityBar, QColor(255,165,0));
        
        ///Erstellt die DistanceBars
        m_p.fillRect(distanceBarLeft, QColor(0, 100, 200));
        m_p.fillRect(distanceBarRight, QColor(0, 100, 200));  
        
        for(QRect r : crossHair)
        {
            m_p.fillRect(r, QColor(0, 100, 0));
        }
        
        ///Schreibt Namen der Spieler in HP-Bars
        m_p.setPen(Qt::white);
        m_p.setFont(QFont("liberation",15,99));
        m_p.drawText(textLeftHP, Qt::AlignCenter,"Player 1");
        m_p.drawText(textRightHP, Qt::AlignCenter,"Player 2");
        
        ///Schreibt die momentane Geschwindigkeit auf die Velocity Bar
        int currentSpeedInt = m_myShip->getCurrentSpeed()/m_myShip->getMaxSpeed() * 100;
        string currentSpeedString = std::to_string(currentSpeedInt) + "%";
        m_p.drawText(textVelocityBar, Qt::AlignCenter, QString::fromStdString(currentSpeedString));

        ///Schreibt Distanz zum Gegner in die textDistanceBar
        int distance_int = distance;
        m_p.drawText(textDistanceBar, Qt::AlignCenter, QString::number(distance_int));
        
        Vector3f diff = m_enemyShip->getPosition() - m_myShip->getPosition();
        Vector3f diffProjected = diff - c * (diff * c * (1/ (c * c)));
        float angleInDegrees = acos(diffProjected * m_myShip->getDirection() / (sqrt(diffProjected * diffProjected) * sqrt(m_myShip->getDirection() * m_myShip->getDirection()))) * 180 / M_PI + 270;
        if(diffProjected * m_myShip->getYAxis() < 0)
        {
            angleInDegrees = 360-angleInDegrees + 180;
        }
        Vector3f b = m_myShip->getXAxis();
        Vector3f diffProjectedYZ = diff - b * (diff * b * (1/ (b*b)));
        if(diffProjectedYZ * c > 0)
        {
            angleInDegrees = angleInDegrees + 180;
            angleInDegrees = 180-angleInDegrees;
        }
        if(diffProjected * m_myShip->getDirection() < 0)
        {
            
            trans.rotate(angleInDegrees);
            m_p.setTransform(trans);
            m_p.drawImage(-m_enemyDirection.width()/2, -m_enemyDirection.height()/2, m_enemyDirection);
        }

        m_p.end();  

        auto now = std::chrono::system_clock::now();
        auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
        auto value = now_ms.time_since_epoch();
        long longNow = value.count();  

        ///Erstellt den Schußanzeige Button
        m_p.begin(this);
        
        ///Kriegen wir vom GLWidget eine -1, wird die Leertaste durchgedrückt
        if(m_lastBulletShot == -1 )
        {
            m_p.setBrush(QColor(255,165,0));
            m_p.drawEllipse(shotReadyRect);
            m_p.setPen(Qt::white);
            m_p.setFont(QFont("liberation",19,99));
            m_p.drawText(shotReadyRect, Qt::AlignCenter,"RAPID");
        }
        ///Sind 500ms vergangengen, darf wieder geschoßen werden
        else if(longNow - m_lastBulletShot > 500)
        {               
            m_p.setBrush(QColor(100,200,0));
            m_p.drawEllipse(shotReadyRect);
            m_p.setPen(Qt::white);
            m_p.setFont(QFont("liberation",19,99));
            m_p.drawText(shotReadyRect, Qt::AlignCenter,"SHOOT");
        }
        ///Sind 500ms noch nicht vergangen, kriegt der Spieler die Nachricht "WAIT"
        else if(longNow - m_lastBulletShot < 500)
        {
            m_p.setBrush(QColor(255,0,0));
            m_p.drawEllipse(shotReadyRect);
            m_p.setPen(Qt::white);
            m_p.setFont(QFont("liberation",20,99));
            m_p.drawText(shotReadyRect, Qt::AlignCenter,"WAIT");
        }
        
        m_p.end();

        ///Gibt eine Siegesnachricht aus, wenn das Gegnerraumschiff zerstört wurde
        if(m_enemyShip->getHP() == 0 && m_myShip->getHP() != 0)
        {
            m_p.begin(this);
            onScreenMessage = QRect(3.75*width()/9, height()/2.7, width()/6 , height()/15);
            m_p.setPen(Qt::blue);
            m_p.setFont(QFont("liberation",30,99));
            m_p.drawText(onScreenMessage, Qt::AlignCenter,"VICTORY");
            m_p.end();
        }
        ///Gibt eine Niederlagenachricht aus, wenn das eigene Schiff zerstört wurde
        else if(m_myShip->getHP() == 0 && m_enemyShip->getHP() != 0)
        {
            m_p.begin(this);
            onScreenMessage = QRect(3.75*width()/9, height()/2.7, width()/6 , height()/15);
            m_p.setPen(Qt::red);
            m_p.setFont(QFont("liberation",30,99));
            m_p.drawText(onScreenMessage, Qt::AlignCenter,"DEFEAT");
            m_p.end();
        } 
        ///Zeigt eine Alarmnachricht wenn die HP unter 3 sind
        else if(m_myShip->getHP() < 3 && m_myShip->getHP() != 0)
        {
                if((longNow - m_lastAlert > 2000 ) || (m_lastAlert + 1000 >= longNow)){
                m_p.begin(this);
                onScreenMessage = QRect(3.75*width()/9, height()/2.7, width()/6 , height()/15);
                m_p.setPen(Qt::red);
                m_p.setFont(QFont("liberation",30,99));
                m_p.drawText(onScreenMessage, Qt::AlignCenter,">> A L E R T <<");
                m_p.end(); 
                if(!(m_lastAlert + 1000 >= longNow))
                {
                    m_lastAlert = longNow;
                }       
            }
        }      
    }

    void HUDWidget::setBulletReady(long lastBulletShot)
    {
        m_lastBulletShot = lastBulletShot;
    }

    void HUDWidget::setFirstPerson(bool b)
    {
        m_firstPerson = b;
    }
}