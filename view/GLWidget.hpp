/*
 *  GLWidget.hpp
 *
 *  Created on: Jan. 14 2019
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __GLWIDGET_HPP__
#define __GLWIDGET_HPP__

#include <string>
#include <chrono>
#include <ctime>

#include <GL/glew.h>
#include <QOpenGLWidget>

#include <limits.h>

#include "rendering/HealthBar.hpp"
#include "rendering/Crosshair.hpp"
#include "view/Camera.hpp"
#include "../rendering/SpaceCraft.hpp"
#include "rendering/Skybox.hpp"
#include "util/AsteroidField.hpp"
#include "../physics/PhysicsEngine.hpp"

using namespace asteroids;
using namespace std::chrono;
using std::shared_ptr;

//typedef std::chrono::milliseconds ms;

/**
 * @brief   implements the OpenGL View in a QT Widget
 *
 */
class GLWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget* parent = NULL);

    /// Parses a level file and loads all supported objects
    void setLevelFile(const std::string& file);

    /// Handles keyboard input and updates the physics engine
    void step(map<Qt::Key, bool>& keyStates);

protected:

    /// Init OpenGL
    virtual void initializeGL() override;

    /// Render scene
    virtual void paintGL() override;

    /// Handle new window dimenions
    virtual void resizeGL(int w, int h) override;

    /// Handle mouse movement
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    // TODO:: Setzen der BulletID passend zur PlayerID. Noch nicht passende Stelle gefunden
    /// Set BulletId according to the playerID
    void setBulletId(int o_id);

private:

    /// Name of the given level file
    string                      m_levelFile;

    /// The virtual camera
    Camera						m_camera;

    /// Health Bar for player 1

    HealthBar::Ptr              m_playerHPBar;

    /// Health Bar for player 2
    
    HealthBar::Ptr              m_enemyHPBar;

    ///Crosshair for aiming

    Crosshair::Ptr              m_crossHair;

    /// A pointer to the Actor
    SpaceCraft::Ptr  	        m_actor;

    /// Zweiter Spieler
    SpaceCraft::Ptr             m_enemyPlayer;

    /// A skybox for the scene
    Skybox::Ptr			        m_skybox;

    /// Pointer to the asteroid field
    AsteroidField::Ptr          m_asteroidField;
    
    /// Physics 
    PhysicsEngine::Ptr          m_physicsEngine;

    /// Rotation speed of the actor
    float                       m_rotationSpeed;

    /// Translation speed of the actor  LevelParser np(m_)s
    float                       m_moveSpeed;

    /// Last set mouse position
    QPoint                      m_mousePos;

    ///Current BulletId
    int m_BulletId;

    /// Letzter Zeitpunkt zu dem eine Kugel abgefeurert wurde
    long                m_lastBullet;

    /// Gibt an nach wie vielen ms wieder geschoßen werden kann
    long                m_schussFrequenz;
};

#endif
