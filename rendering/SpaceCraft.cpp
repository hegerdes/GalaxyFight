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

SpaceCraft::~SpaceCraft()
{
 
}

} // namespace asteroids
