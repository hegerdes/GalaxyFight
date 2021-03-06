#include "GLWidget.hpp"
#include "../io/LevelParser.hpp"
#include "global_socket.h"
#include "io/TextureFactory.hpp"
#include <QMouseEvent>
#include "global_socket.h"

#include <SDL2/SDL.h>

GLWidget::GLWidget(QWidget* parent)
//m_camera(Vector3f(0.0f, 0.0f, -700.0f), 0.05f, 5.0f)
    : QOpenGLWidget(parent),
      m_camera(),
      m_rotationSpeed(0.025),
      m_moveSpeed(5.0),
      m_lastBullet(0),
      active(false),
      m_schussFrequenz(500), 
      m_firstPerson(false), 
      m_firstPersonAble(true), 
      m_hud(this), 
      m_rapidFire(false)
{
    m_layout.addWidget(&m_hud);
    setLayout(&m_layout);
    //Notwendig, damit Qt nicht Leertaste und Pfeiltasteneingaben frisst
    setFocusPolicy(Qt::StrongFocus);
}

void GLWidget::setLevelFile(const std::string& file) { m_levelFile = file; }

void GLWidget::initializeGL() {
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Setup two light sources
    float light0_position[4];
    float light0_ambient[4];
    float light0_diffuse[4];

    float light1_position[4];
    float light1_ambient[4];
    float light1_diffuse[4];

    light0_position[0] = 1.0f;
    light0_ambient[0] = 0.8f;
    light0_diffuse[0] = 0.8f;
    light0_position[1] = 1.0f;
    light0_ambient[1] = 0.8f;
    light0_diffuse[1] = 0.8f;
    light0_position[2] = 0.0f;
    light0_ambient[2] = 0.8f;
    light0_diffuse[2] = 0.8f;
    light0_position[3] = 1.0f;
    light0_ambient[3] = 0.1f;
    light0_diffuse[3] = 1.0f;

    light1_position[0] = 0.0f;
    light1_ambient[0] = 0.1f;
    light1_diffuse[0] = 0.5f;
    light1_position[1] = -1.0f;
    light1_ambient[1] = 0.1f;
    light1_diffuse[1] = 0.5f;
    light1_position[2] = 0.0f;
    light1_ambient[2] = 0.1f;
    light1_diffuse[2] = 0.5f;
    light1_position[3] = 1.0f;
    light1_ambient[3] = 1.0f;
    light1_diffuse[3] = 1.0f;

    // Light 1
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glEnable(GL_LIGHT0);

    // Light 2
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glEnable(GL_LIGHT1);

    // Enable lighting
    glEnable(GL_LIGHTING);

    // Enable z buffer and gouroud shading
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glShadeModel(GL_SMOOTH);

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

void GLWidget::loadLevel() {
    // Load level
    // LevelParser lp(m_levelFile, m_actor, m_enemyPlayer, m_skybox, m_asteroidField);
    LevelParser lp("./models/level.xml", m_actor, m_enemyPlayer, m_skybox, m_asteroidField);

    // Setup physics//
    m_physicsEngine = make_shared<PhysicsEngine>();

    m_playerHPBar = make_shared<HealthBar>(HealthBar::PLAYER_1, width(), height(), 10);

    m_enemyHPBar = make_shared<HealthBar>(HealthBar::PLAYER_2, width(), height(), 10);

    m_crossHair = make_shared<Crosshair>(0.0f, 100.0f / 255.0f, 0.0f, width(), height());

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

    // Fügt das Raumschiff der Engine hinzu, damit es richtig explodieren kann

    m_physicsEngine->addSpaceCraft(m_actor);
    m_physicsEngine->addEnemyPlayer(m_enemyPlayer);
    m_hud.setSpacecraft(m_actor);
    m_hud.setEnemy(m_enemyPlayer);
    // setzen dummy variablen
    hp_actor = 10;
    hp_enemy = 10;
    // active = true;
}

void GLWidget::paintGL() {
    if (active) {
        // Clear bg color and enable depth test (z-Buffer)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        m_camera.follow(m_actor);
        m_camera.apply();

        // Render stuff
        m_skybox->render(m_camera);

        // Render all physical objects
        m_physicsEngine->render();

        if (!m_firstPerson) {
            m_actor->render();
        }
        m_enemyPlayer->render();

        // Debug/Testline
        // m_enemyPlayer->setPosition(Vector<float>(100,100,100));
        //    m_playerHPBar->render();

        //  m_enemyHPBar->render();

        //    m_crossHair->render();
    }
}

void GLWidget::step(map<Qt::Key, bool>& keyStates) {
    if (active) {
        // std::cerr << "\t" << __LINE__ << __PRETTY_FUNCTION__ << "\n";
        // Get keyboard states and handle model movement
        m_physicsEngine->process();

        Bullet_shot bullet_shot = Bullet_shot::not_shot;
        if (m_actor->spaceCraftStatus() == 0) {

            m_actor->move(Transformable::FORWARD, m_actor->getCurrentSpeed());
            // Toggle first person
            if (keyStates[Qt::Key_V]) {
                if (m_firstPersonAble) {
                    m_firstPerson = !m_firstPerson;
                    m_hud.setFirstPerson(m_firstPerson);
                    m_camera.setFirstPerson(m_firstPerson);
                    m_firstPersonAble = false;
                }
            } else {
                m_firstPersonAble = true;
            }

            if (keyStates[Qt::Key_Up]) {
                m_actor->rotate(Transformable::PITCH_RIGHT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_Down]) {
                m_actor->rotate(Transformable::PITCH_LEFT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_Left]) {
                m_actor->rotate(Transformable::ROLL_LEFT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_Right]) {
                m_actor->rotate(Transformable::ROLL_RIGHT, m_rotationSpeed);
            }

            if (keyStates[Qt::Key_W]) {
                m_actor->accelerate();
                if(!SoundFactory::instance().getSound("schneller")->isPlaying())
                {
                    SoundFactory::instance().getSound("schneller")->play();
                }
                 
            }
            if (keyStates[Qt::Key_S]) {
                m_actor->deccelerate();
                if(!SoundFactory::instance().getSound("langsamer")->isPlaying())
                {
                    SoundFactory::instance().getSound("langsamer")->play();
                }
            }
            if (keyStates[Qt::Key_A]) {
                m_actor->rotate(Transformable::YAW_LEFT, m_rotationSpeed);
            }
            if (keyStates[Qt::Key_D]) {
                m_actor->rotate(Transformable::YAW_RIGHT, m_rotationSpeed);
            }

            if(keyStates[Qt::Key_X])
            {
                //Debug/Testline für Explosion eigenes Raumschiff
                m_actor->destroySpaceCraft();
            }


            // Add a bullet to physics engine
            if (keyStates[Qt::Key_Space]) {

                // std::cerr<< "bullet shot 1";
                auto now = std::chrono::system_clock::now();
                auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
                auto value = now_ms.time_since_epoch();
                long bulletShot = value.count();
                /// Ermittelt, wann die letzte Kugel abgeschoßen wurde und erlaubt erst nach
                if (bulletShot - m_lastBullet > m_schussFrequenz) {
                    m_BulletId++;
                    Vector3f shipPosition =
                        m_actor->getPosition() + m_actor->getZAxis() * -45 + m_actor->getXAxis() * -175;
                    Bullet::Ptr bullet = make_shared<Bullet>(Bullet(shipPosition, m_actor->getDirection()));
                    bullet->setid(m_BulletId);
                    m_physicsEngine->addBullet(bullet);
                    m_lastBullet = bulletShot;
                    bullet_shot = Bullet_shot::shot;
                    std::cerr << "bullet shot ID::::: " << m_BulletId;

                    ///Gibt der Anzeige in m_hud die Zeit an der die letzte Bullet abgeschoßen wurde
                    ///
                    m_hud.setBulletReady(value.count());   


                    ///Spielt den Schusssound ab
                    SoundFactory::instance().getSound("pew")->play();                

                    ///Der Folgende Codeblock liefert dem shotReadyRect in HUDWidget die notwendigen Informationen, damit es
                    ///korrekt anzeigen kann, ob das Schiff bereit zum schießen ist
                    //Start Rapidfire Anzeigeinformationen
                    if(m_rapidFire)
                    {                        
                        m_hud.setBulletReady(-1); 
                    }                                                     

                                     
                }else
                {
                    m_rapidFire= true;
                }                

            }else
            {
                m_rapidFire = false;
                m_hud.setBulletReady(0);
            }
            // Ende Rapidfire Anzeigeinformationen
        }
        // TestKnopf zum zerstören vom Gegnerschiff
        if (keyStates[Qt::Key_Y]) {
            m_enemyPlayer->destroySpaceCraft();
        }

        // Trigger update, i.e., redraw via paintGL()
        /*client_global.sendUpdate_3D_C(m_actor->m_position, m_actor->m_xAxis, m_actor->m_yAxis, m_actor->m_zAxis,
                                      bullet_shot, Living::alive, m_BulletId);
        client_global.readData();
        */
        if (client_global.init_received) {
            m_physicsEngine->reset();
            m_actor->m_position = client_global.ownPos;
            m_actor->m_xAxis = client_global.ownxAxis;
            m_actor->m_yAxis = client_global.ownyAxis;
            m_actor->m_zAxis = client_global.ownzAxis;

            m_actor->m_hp = 10;
            m_enemyPlayer->m_hp = 10;
            m_enemyPlayer->m_status = 0;
            m_actor->m_status = 0;

            // asteroids hinzufügen

            // manipulate asteroids depending on the data that the server has sent
            int i = 0;
            std::list<Asteroid::Ptr> asteroids;
            m_asteroidField->getAsteroids(asteroids);
            //std::cerr << "\t" << __FUNCTION__<< client_global.count_astr << " ----------------------------------\n";
            for (auto it = asteroids.begin(); it != asteroids.end(); it++)
            {
                if(i < client_global.count_astr)
                {
                    //manipulate position,size and direction of asteroids
                    (*it)->m_position = client_global.pos_astr[i];
                    // std::cerr << "m_position " << (*it)->m_position << "\n";
                    (*it)->m_radius = client_global.size_astr[i];
                    // std::cerr << "m_radius " << (*it)->m_radius << "\n";
                    (*it)->m_direction = client_global.dir_astr[i];
                    // std::cerr << "m_direction " << (*it)->m_direction << "\n";
                    (*it)->setScale(client_global.size_astr[i]);
                    // TODO SET ID
                    Asteroid::Ptr p = *it;
                    p->setid(client_global.id_astr[i]);
                    m_physicsEngine->addDestroyable(p);
                    i++;
                    // std::cerr << "Asteroid entpackt\n";

                } else {
                    // std::cerr << "XML asterooid ignoriert.";
                }

                // PhysicalObject::Ptr p = std::static_pointer_cast<PhysicalObject>(*it);
            }

            client_global.init_received = false;
            // std::cerr << "erhaletn----------------------------------------------\n";

            // TODO: TESTING __________________________________________________
            if (client_global.player_No == player_no::first) {
                m_BulletId = INT_MIN;
            } else {
                m_BulletId = 0;
            }

            // std::cerr << "SET OWN BULLET ID: " << __LINE__ <<  "=======" << client_global.player_No << "++++++++" <<
            // m_BulletId;

            // std::cerr << "\t" << __FUNCTION__ << __LINE__ <<"\n";
        } else if (m_actor->getHP() < 1 || m_enemyPlayer->getHP() < 1) {
            // std::cerr << "\t" << __FUNCTION__<< __LINE__ <<"\n";
            player_no player_3d_winner;
            if (m_actor->getHP() < 1) {
                player_3d_winner = player_no::loser;
            } else {
                player_3d_winner = player_no::winner;
            }
            client_global.send_end_3d(player_3d_winner);

            client_global.wait_for_readData(20);
            // client_global.readData();
        } else {
            // std::cerr << "\t" << __FUNCTION__<< __LINE__ <<"\n";
            client_global.sendUpdate_3D_C(m_actor->m_position, m_actor->m_xAxis, m_actor->m_yAxis, m_actor->m_zAxis,
                                          bullet_shot, Living::alive, m_BulletId);
            client_global.wait_for_readData(20);
        }


        m_enemyPlayer->m_position = client_global.enemyPos;
        m_enemyPlayer->m_xAxis = client_global.enemyxAxis;
        m_enemyPlayer->m_yAxis = client_global.enemyyAxis;
        m_enemyPlayer->m_zAxis = client_global.enemyzAxis;

        m_enemyPlayer->setHealth(client_global.enemy_health);
        m_actor->setHealth(client_global.own_health);

        if (client_global.enemy_shot == Bullet_shot::shot) {
            Vector3f shipPosition = m_enemyPlayer->getPosition() + m_enemyPlayer->getZAxis() * -45 + m_enemyPlayer->getXAxis() * -175;
            Bullet::Ptr bullet = make_shared<Bullet>(Bullet(shipPosition, m_enemyPlayer->m_xAxis * -1));
            bullet->setid(client_global.enemy_shot_id);
            m_physicsEngine->addBullet(bullet);
            client_global.enemy_shot = Bullet_shot::not_shot;
        }

        if (hp_actor != m_actor->getHP()) {
            // std::cout << "Own Health: " << m_actor->getHP() << std::endl;
            hp_actor = m_actor->getHP();
        }

        if (hp_enemy != m_enemyPlayer->getHP()) {
            // std::cout << "Enemy Health: " << m_enemyPlayer->getHP() << std::endl;
            hp_enemy = m_enemyPlayer->getHP();
        }

        m_enemyHPBar->setHP(m_enemyPlayer->getHP());
        m_playerHPBar->setHP(m_actor->getHP());
        this->update();
        m_hud.update();

        // std::cerr << "\t" << __FUNCTION__<< __LINE__ <<"\n";
        if (client_global.m_planet_changes_received) {
            active = false;
            client_global.m_planet_changes_received = false;
            // std::cerr << "\t" << __FUNCTION__<< __LINE__ <<" m_planet_changes_received\n";
            emit planet_apply_updates();
            // std::cerr << "\t" << __FUNCTION__<< __LINE__ <<" m_planet_changes_received\n";
            emit goToScene2D();
            // std::cerr << "\t" << __FUNCTION__<< __LINE__ <<" m_planet_changes_received\n";
        }
    }
}

void GLWidget::mouseMoveEvent(QMouseEvent* event) {

}

void GLWidget::resizeGL(int w, int h) {
    float ratio = width() * 1.0 / height();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width(), height());
    gluPerspective(45, ratio, 1, 10000);

    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::setBulletId(int o_id) { m_BulletId = o_id; }
