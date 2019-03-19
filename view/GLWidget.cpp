#include "GLWidget.hpp"
#include "../io/LevelParser.hpp"
#include "io/TextureFactory.hpp"
#include <QMouseEvent>
//#include "global_socket.h"

#include <SDL2/SDL.h>

GLWidget::GLWidget(QWidget* parent)
    : QOpenGLWidget(parent),
      m_camera(Vector3f(0.0f, 0.0f, -700.0f), 0.05f, 5.0f),
      m_rotationSpeed(0.02),
      m_moveSpeed(1.0)
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

    // Load level
    LevelParser lp(m_levelFile, m_actor, m_enemyPlayer, m_skybox, m_asteroidField);

    // Setup physics
    m_physicsEngine = make_shared<PhysicsEngine>();

    // Add asteroids to physics engine
    std::list<Asteroid::Ptr> asteroids;
    m_asteroidField->getAsteroids(asteroids);
    for (auto it = asteroids.begin(); it != asteroids.end(); it++)
    {
        PhysicalObject::Ptr p = std::static_pointer_cast<PhysicalObject>(*it);
        m_physicsEngine->addDestroyable(p);
    }

    // @ahaker
    //socket.connectToHost(QHostAddress::LocalHost, 38291);
    socket.connectToHost("lennartkaiser.de", 38291);
    std::cerr << socket.waitForConnected() << ": socket.waitForConnected\n";

}

void GLWidget::paintGL()
{
    // Clear bg color and enable depth test (z-Buffer)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_camera.apply();

    // Render stuff
    m_skybox->render(m_camera);

    // Render all physical objects
    m_physicsEngine->render();

    m_actor->render();

    m_enemyPlayer->render();
    m_enemyPlayer->setPosition(Vector<float>(10,100,10));
}

QByteArray IntToArray(qint32 source) //Use qint32 to ensure that the number have 4 bytes
{
    //Avoid use of cast, this is the Qt way to serialize objects
    QByteArray temp;
    QDataStream data(&temp, QIODevice::ReadWrite);
    data << source;
    return temp;
}

void GLWidget::step(map<Qt::Key, bool>& keyStates)
{
    // Get keyboard states and handle model movement
    m_physicsEngine->process();

    if (keyStates[Qt::Key_Up])
    {
        // Quaternion nq;
        // float s = m_enemyPlayer->getSpeed();
        // Vector3f xAxis = m_enemyPlayer->getXAxis();
        // Vector3f yAxis = m_enemyPlayer->getYAxis();
        // Vector3f zAxis = m_enemyPlayer->getZAxis();
        // nq.fromAxis(xAxis, s);
        // yAxis = nq * yAxis;
        // zAxis = nq * zAxis;

        m_actor->rotate(Transformable::YAW_LEFT, m_rotationSpeed);
        // m_enemyPlayer->setTransformationMatrix(xAxis,yAxis,zAxis,m_enemyPlayer->getPosition());
    }
    if (keyStates[Qt::Key_Down])
    {
        m_actor->rotate(Transformable::YAW_RIGHT, m_rotationSpeed);
    }
       if (keyStates[Qt::Key_Left])
    {
        m_actor->rotate(Transformable::ROLL_LEFT, m_rotationSpeed);
    }
    if (keyStates[Qt::Key_Right])
    {
        m_actor->rotate(Transformable::ROLL_RIGHT, m_rotationSpeed);
    }
   
    if (keyStates[Qt::Key_W])
    {
        m_actor->move(Transformable::FORWARD, m_moveSpeed);
    }
    if (keyStates[Qt::Key_S])
    {
        m_actor->move(Transformable::BACKWARD, m_moveSpeed);
    }
    if (keyStates[Qt::Key_A])
    {
        m_actor->move(Transformable::STRAFE_LEFT, m_moveSpeed);
    }
    if (keyStates[Qt::Key_D])
    {
        m_actor->move(Transformable::STRAFE_RIGHT, m_moveSpeed);
    }

    // Add a bullet to physics engine
    if(keyStates[Qt::Key_Space])
    {
        Bullet::Ptr bullet = make_shared<Bullet>(Bullet(m_actor->getPosition(), m_actor->getDirection()));
        m_physicsEngine->addBullet(bullet);
    }

    // Trigger update, i.e., redraw via paintGL()
    // @ahaker
    auto start = chrono::high_resolution_clock::now();
    if(socket.state() == QAbstractSocket::ConnectedState)
    {
        // wenn socket verbunden -> sende deine eigenen daten
        std::cout << "write data\n";
        QByteArray data;

        /*
            char * position_chars = (char*)&m_actor->m_position;
            char * xaxis_temp = (char*)&m_actor->m_xAxis;
            char * yaxis_temp = (char*)&m_actor->m_yAxis;
            char * zaxis_temp = (char*)&m_actor->m_zAxis;
            data.append(position_chars, 3*4);
            data.append(xaxis_temp, 3*4);
            data.append(yaxis_temp, 3*4);
            data.append(zaxis_temp, 3*4);
        */

            /*
        float float_temp [12] = {0};
        char * float_char = (char*)float_temp;
        data.append(float_char, 12*4);
        */
        /*
            float * flt_prt = (float*)data.data();
            cout << "Data: ";
            for(size_t count{0}; count < 19; count++)
            {
                cout << flt_prt[count] << ",";
            }
            cout << "\n";
        */

        //data.append((char*)(&position),(sizeof(position[0]*3)));
        socket.write(IntToArray(data.size())); //write size of data
        socket.write(data);
        // empfange die positionen des anderen

        //#########computeMatrix ist nocht auskommentiert
        QByteArray answer = socket.readAll();
        if( 12*4 == answer.length()){
            std::cerr << socket.waitForBytesWritten() << "; waitForBytesWritten\n";
            float* position_temp = (float*) answer.data();
            asteroids::Vector<float,3> position = m_actor->getPosition();
            // checke ob die empfangene positionen zu weit von den vorheriegen
            // positionen abweichen, wenn ja nutze die alte
            /*
            for(int count {0}; count < 3; count++)
                {
                   if(((position[count]-position_temp[count]) < -5.0f) |
                      ((position[count]-position_temp[count]) > 5.0f))
                   {
                       position_temp[count] = position[count];
                       std::cout << "---------------------------------\n";
                   }
                }
            */
            /*
            m_enemyPlayer->setPosition({position_temp[0], position_temp[1], position_temp[2]});

            m_enemyPlayer->m_xAxis[0] = position_temp[3];
            m_enemyPlayer->m_xAxis[1] = position_temp[4];
            m_enemyPlayer->m_xAxis[2] = position_temp[5];

            m_enemyPlayer->m_yAxis[0] = position_temp[6];
            m_enemyPlayer->m_yAxis[1] = position_temp[7];
            m_enemyPlayer->m_yAxis[2] = position_temp[8];

            m_enemyPlayer->m_zAxis[0] = position_temp[9];
            m_enemyPlayer->m_zAxis[1] = position_temp[10];
            m_enemyPlayer->m_zAxis[2] = position_temp[11];
            */

            /*
                asteroids::Matrix m_neu;
                cout << "Received: ";
                for(int count{0}; count < 16+3; count++){
                    cout << position_temp[count] << ",";
                }
                cout << "\n";

                for(int count{3}; count < 16+3; count++){
                    m_neu[count-3] = position_temp[count];
                }
                m_enemyPlayer->m_transformation = m_neu;
                cout << "m_transformation: ";
                std::cout<<position_temp[0]<<","<<position_temp[1]<<","<<position_temp[2]<<"; ";
                //m_enemyPlayer->m_xAxis[0] = m_enemyPlayer->m_transformation[0];
                for(int count{0}; count < 16; count++){
                    cout << m_enemyPlayer->m_transformation[count] << "," ;
                }
                cout << "\n";
            */

        }
    }
    cout << chrono::duration<double, milli>
                    (chrono::high_resolution_clock::now()-start).count() << "; round time\n";
    this->update();
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
    bool l_pressed = (event->buttons() & Qt::LeftButton) != 0;
    bool r_pressed = (event->buttons() & Qt::RightButton) != 0;

    QPoint delta = event->pos() - m_mousePos;
    m_mousePos = event->pos();

    // Handle motion for pressed L button while R is not
    // pressed
    if (l_pressed & !r_pressed)
    {
        if (delta.x() > -3)
        {
            m_camera.turn(Camera::RIGHT);
        }
        if (delta.x() < 3)
        {
            m_camera.turn(Camera::LEFT);
        }
        if (delta.y() > 3)
        {
            m_camera.turn(Camera::UP);
        }
        if (delta.y() < -3)
        {
            m_camera.turn(Camera::DOWN);
        }
    }

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
        if (delta.y() > 3)
        {
            m_camera.move(Camera::FORWARD);
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
