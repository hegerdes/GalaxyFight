/*
 *  SpaceCraft.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include "SpaceCraft.hpp"
#include "io/TriangleMeshFactory.hpp"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace asteroids
{

SpaceCraft::SpaceCraft(const std::string &filename, const Vector3f& position, float movespeed, float rotatespeed)
{
    m_mesh = TriangleMeshFactory::instance().getMesh(filename);
    m_speed = movespeed;
    m_maxSpeed = movespeed;
    m_currentSpeed = 0;
    m_rotationSpeed = rotatespeed;
    m_status = 0;
    m_alive = true;

    m_hp = 10;
    if(m_mesh)
    {
       setPosition(position);
    }
}


void SpaceCraft::render()
{
    // Compute transformation matrix
    if(m_status==0){
        computeMatrix();

        glPushMatrix();
        glMultMatrixf(m_transformation.getData());
        m_mesh->render();
        glPopMatrix();
    }
}

bool SpaceCraft::hasMesh() const
{
    return m_mesh != nullptr;
}

void SpaceCraft::destroySpaceCraft()
{
    if(m_status != 2){
        m_status = 1;
        m_hp = 0;
    }
}

int SpaceCraft::spaceCraftStatus()
{
    return m_status;
}

void SpaceCraft::endDestruction()
{
    m_status = 2;
}
void SpaceCraft::accelerate()
{
    m_currentSpeed += m_maxSpeed * 0.025;
    if(m_currentSpeed >= m_maxSpeed)
    {
        m_currentSpeed = m_maxSpeed;
    }
}

void SpaceCraft::deccelerate()
{
    m_currentSpeed -= m_maxSpeed * 0.025;
    if(m_currentSpeed <= 0)
    {
        m_currentSpeed = 0;
    }

}

float SpaceCraft::getCurrentSpeed()
{
    return m_currentSpeed;
}

float SpaceCraft::getMaxSpeed()
{
    return m_maxSpeed;
}

//TODO: Feinschliff für Hitboxen
//vec Position des Objektes, r ist radius
bool SpaceCraft::collideShip(Vector<float> vec, float r)
{

    if(!m_alive)
    {
        return false;
    }

    Vector<float> axis = getXAxis();
    Vector<float> xAxis = getXAxis();
    Vector<float> yAxis = getYAxis();

    Vector<float> pos = m_position;
    Vector<float> dist = pos - vec;
    
    Vector<float> center_box = pos;
    dist = center_box - vec;
    float distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 50, 2))
    {
        std::cout << "CENTER HIT" << std::endl;
        return true;
    }
    
    Vector<float> back_box = pos + xAxis * 90;
    dist = back_box - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 50, 2))
    {
        std::cout << "BACK HIT" << std::endl;
        return true;
    }

    Vector<float> back_box_left = back_box + yAxis * 70;
    dist = back_box_left - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 55, 2))
    {
        std::cout << "BACK LEFT HIT" << std::endl;
        return true;
    }

    Vector<float> back_box_right = back_box - yAxis * 70;
    dist = back_box_right - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 55, 2))
    {
        std::cout << "BACK RIGHT HIT" << std::endl;
        return true;
    }
   
    Vector<float> farback_box = pos + xAxis * 110;
    dist = farback_box - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 50, 2))
    {
        std::cout << "FAR-BACK HIT" << std::endl;
        return true;
    }

    Vector<float> farfront_box = pos - xAxis * 70;
    dist = farfront_box - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 50, 2))
    {
        std::cout << "FRONT HIT" << std::endl;
        return true;
    }

    Vector<float> front_box = pos - xAxis * 100;
    dist = front_box - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 50, 2))
    {
        std::cout << "FAR-FRONT HIT" << std::endl;
        return true;
    }
    


    return false;
}

void SpaceCraft::gotHit()
{
    m_hp--;

    if(m_hp == 0)
    {
         m_alive = false;
         destroySpaceCraft();
         
    }
}

void SpaceCraft::setHealth(int health){
    m_hp = health;

    if(m_hp == 0)
    {
         m_alive = false;
         destroySpaceCraft();
    }
}

SpaceCraft::~SpaceCraft()
{
 
}

} // namespace asteroids
