#include "GLWidget.hpp"
#include "../io/LevelParser.hpp"
#include "io/TextureFactory.hpp"
#include <QMouseEvent>
#include "global_socket.h"

#include <SDL2/SDL.h>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      m_camera(Vector3f(0.0f, 0.0f, -700.0f), 0.05f, 5.0f),
      m_rotationSpeed(0.025),
      m_moveSpeed(5.0),
      m_lastBullet(0),
      m_schussFrequenz(500),
      active(false)
{
}

void GLWidget::setLevelFile(const std::string& file)
{
    m_levelFile = file;
}

void GLWidget::initializeGL()
{
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
#endif
    // Init OpenGL projection matrix
    glClearColor(0.0, 0.0, 0.0, 1.0);
    float ratio = width() * 1.0 / height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    gluPerspective(45, ratio, 1, 10000);

    // Enter model view mode
    glMatrixMode(GL_MODELVIEW);
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);

    // Setup two light sources
    float light0_position[4];
    float light0_ambient[4];
    float light0_diffuse[4];

    float light1_position[4];
    float light1_ambient[4];
    float light1_diffuse[4];

    light0_position[0] =   1.0f;
    light0_ambient[0] = 0.8f;
    light0_diffuse[0] = 0.8f;
    light0_position[1] =   1.0f;
    light0_ambient[1] = 0.8f;
    light0_diffuse[1] = 0.8f;
    light0_position[2] =   0.0f;
    light0_ambient[2] = 0.8f;
    light0_diffuse[2] = 0.8f;
    light0_position[3] =   1.0f;
    light0_ambient[3] = 0.1f;
    light0_diffuse[3] = 1.0f;

    light1_position[0] =   0.0f;
    light1_ambient[0] = 0.1f;
    light1_diffuse[0] = 0.5f;
    light1_position[1] =  -1.0f;
    light1_ambient[1] = 0.1f;
    light1_diffuse[1] = 0.5f;
    light1_position[2] =   0.0f;
    light1_ambient[2] = 0.1f;
    light1_diffuse[2] = 0.5f;
    light1_position[3] =   1.0f;
    light1_ambient[3] = 1.0f;
    light1_diffuse[3] = 1.0f;

    // Light 1
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);

    // Light 2
    glLightfv(GL_LIGHT1, GL_AMBIENT,  light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE,  light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Enable z buffer and gouroud shading
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel (GL_SMOOTH);


    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 3.2 is part of the modern versions of OpenGL,
    // but most video cards whould be able to run it
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    loadLevel();
}
void GLWidget::loadLevel()
{
    active = true;
    // Load level
    //LevelParser lp(m_levelFile, m_actor, m_enemyPlayer, m_skybox, m_asteroidField); @ahaker
    LevelParser lp("./models/level.xml", m_actor, m_enemyPlayer, m_skybox, m_asteroidField);



    // Setup physics//
    m_physicsEngine = make_shared<PhysicsEngine>();

    m_playerHPBar = make_shared<HealthBar>(HealthBar::PLAYER_1, width(), height(), 10);

    m_enemyHPBar = make_shared<HealthBar>(HealthBar::PLAYER_2, width(), height(), 10);

    m_crossHair = make_shared<Crosshair>(0.0f, 100.0f/255.0f, 0.0f, width(), height());

    // Add asteroids to physics engine

    /* MANAGED IN init_3d
    std::list<Asteroid::Ptr> asteroids;
    m_asteroidField->getAsteroids(asteroids);
    for (auto it = asteroids.begin(); it != asteroids.end(); it++)
    {
        PhysicalObject::Ptr p = std::static_pointer_cast<PhysicalObject>(*it);
        m_physicsEngine->addDestroyable(p);
    } 
    }
    */

    //Fügt das Raumschiff der Engine hinzu, damit es richtig explodieren kann
    m_physicsEngine->addSpaceCraft(m_actor);
    m_physicsEngine->addEnemyPlayer(m_enemyPlayer);
}

void GLWidget::paintGL()
{
    if(active)
    {
    // Clear bg color and enable depth test (z-Buffer)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_camera.follow(m_actor);
    m_camera.apply();

        // Render stuff
        m_skybox->render(m_camera);

        // Render all physical objects
        m_physicsEngine->render();

    m_actor->render();

    m_enemyPlayer->render();

    //Debug/Testline
    //m_enemyPlayer->setPosition(Vector<float>(10,100,10));
    m_playerHPBar->render();

    m_enemyHPBar->render();

    m_crossHair->render();
    }
}

void GLWidget::step(map<Qt::Key, bool>& keyStates)
{
    if(active){
        // Get keyboard states and handle model movement
        m_physicsEngine->process();


        Bullet_shot bullet_shot = Bullet_shot::not_shot;
        if(m_actor->spaceCraftStatus() == 0){


            m_actor->move(Transformable::FORWARD, m_actor->getCurrentSpeed());

            if (keyStates[Qt::Key_L])
            {
                m_actor->rotate(Transformable::PITCH_RIGHT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_H])
            {
                m_actor->rotate(Transformable::PITCH_LEFT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_J])
            {
                m_actor->rotate(Transformable::ROLL_LEFT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_K])
            {
                m_actor->rotate(Transformable::ROLL_RIGHT, m_rotationSpeed);
            }

            if (keyStates[Qt::Key_W])
            {
                m_actor->accelerate();
            }
            if (keyStates[Qt::Key_S])
            {
                m_actor->deccelerate();
            }
            if (keyStates[Qt::Key_A])
            {
                m_actor->rotate(Transformable::YAW_LEFT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_D])
            {
                m_actor->rotate(Transformable::YAW_RIGHT, m_rotationSpeed);
            }


                if(keyStates[Qt::Key_X]){
                    //Debug/Tesline für Explosion eigenes Raumschiff
                    m_actor->destroySpaceCraft();
                }

                // Add a bullet to physics engine
                if(keyStates[Qt::Key_N])
                {

                    auto now = std::chrono::system_clock::now();
                    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
                    auto value = now_ms.time_since_epoch();
                    long bulletShot = value.count();
                    ///Ermittelt, wann die letzte Kugel abgeschoßen wurde und erlaubt erst nach
                    if(bulletShot - m_lastBullet > m_schussFrequenz){
                        Vector3f shipPosition = m_actor->getPosition() + m_actor->getZAxis() * -45 + m_actor->getXAxis() * -175;
                        Bullet::Ptr bullet = make_shared<Bullet>(Bullet(shipPosition, m_actor->getDirection()));
                        m_physicsEngine->addBullet(bullet);
                        m_lastBullet = bulletShot;
                        bullet_shot = Bullet_shot::shot;
                    }

                }
        }
        if(keyStates[Qt::Key_Y]){
                //Debug/Tesline für Explosion eigenes Raumschiff
                m_enemyPlayer->destroySpaceCraft();
            }
        // Trigger update, i.e., redraw via paintGL()
        if(client_global.init_received)
        {
            m_actor->m_position = client_global.ownPos;
            m_actor->m_xAxis = client_global.ownxAxis;
            m_actor->m_yAxis = client_global.ownyAxis;
            m_actor->m_zAxis = client_global.ownzAxis;
            // asteroids hinzufügen

            int i = 0;
            std::list<Asteroid::Ptr> asteroids;
            m_asteroidField->getAsteroids(asteroids);
            std::cerr << client_global.count_astr << " ----------------------------------\n";
            for (auto it = asteroids.begin(); it != asteroids.end(); it++)
            {
                if(i < client_global.count_astr)
                {
                    (*it)->m_position = client_global.pos_astr[i];
                    std::cerr << "m_position " << (*it)->m_position << "\n";
                    (*it)->m_radius = client_global.size_astr[i];
                    std::cerr << "m_radius " << (*it)->m_radius << "\n";
                    (*it)->m_direction = client_global.dir_astr[i];
                    std::cerr << "m_direction " << (*it)->m_direction << "\n";
                    //TODO SET ID
                }
                i++;
                PhysicalObject::Ptr p = std::static_pointer_cast<PhysicalObject>(*it);
                m_physicsEngine->addDestroyable(p);
                std::cout << i << ". Asteroid entpackt!\n";
            }

            client_global.init_received = false;
            std::cerr << "erhalten----------------------------------------------\n";
        } else {
            client_global.sendUpdate_3D_C(m_actor->m_position, m_actor->m_xAxis,
                                         m_actor->m_yAxis, m_actor->m_zAxis,
                                         bullet_shot, Living::alive, 0);
            client_global.readData();
        }

        m_enemyPlayer->m_position = client_global.enemyPos;
        m_enemyPlayer->m_xAxis = client_global.enemyxAxis;
        m_enemyPlayer->m_yAxis = client_global.enemyyAxis;
        m_enemyPlayer->m_zAxis = client_global.enemyzAxis;

        if(client_global.enemy_shot == Bullet_shot::shot)
        {
            Vector3f shipPosition = m_enemyPlayer->getPosition() + m_enemyPlayer->getZAxis() * -45 + m_enemyPlayer->getXAxis() * -175;
            Bullet::Ptr bullet = make_shared<Bullet>(Bullet(shipPosition, m_enemyPlayer->m_xAxis*-1));
            m_physicsEngine->addEnemyBullet(bullet);
            client_global.enemy_shot = Bullet_shot::not_shot;
        }

        //std::cout << "Own Health: " << m_actor->getHP() << std::endl;
        //std::cout << "Enemy Health: " << m_enemyPlayer->getHP() << std::endl;
        m_enemyHPBar->setHP(m_enemyPlayer->getHP());
        m_playerHPBar->setHP(m_actor->getHP());
        this->update();
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    bool l_pressed = (event->buttons() & Qt::LeftButton) != 0;
    bool r_pressed = (event->buttons() & Qt::RightButton) != 0;

    QPoint delta = event->pos() - m_mousePos;
    m_mousePos = event->pos();

    // Handle motion for pressed L button while R is not
    // pressed
    // if (l_pressed & !r_pressed)
    // {
    //     if (delta.x() > -3)
    //     {
    //         m_camera.turn(Camera::RIGHT);
    //     }
    //     if (delta.x() < 3)
    //     {
    //         m_camera.turn(Camera::LEFT);
    //     }
    //     if (delta.y() > 3)
    //     {
    //         m_camera.turn(Camera::UP);
    //     }
    //     if (delta.y() < -3)
    //     {
    //         m_camera.turn(Camera::DOWN);
    //     }
    // }

    // Handle motion for pressed R button while L is not
    // pressed
    if (r_pressed & !l_pressed)
    {
        if (delta.x() > 3)
        {
            m_camera.move(Camera::LEFT);
        }
        if (delta.x() < -3)
        {
            m_camera.move(Camera::RIGHT);
        }
        if (delta.y() < -3)
        {
            m_camera.move(Camera::BACKWARD);
        }
    }
}

void GLWidget::resizeGL(int w, int h)
{
    float ratio = width() * 1.0 / height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    gluPerspective(45, ratio, 1, 10000);

    glMatrixMode(GL_MODELVIEW);
}
