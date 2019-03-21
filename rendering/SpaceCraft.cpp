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
    m_rotationSpeed = rotatespeed;
    m_hp = 10;
    if(m_mesh)
    {
       setPosition(position);
    }
}


void SpaceCraft::render()
{
    // Compute transformation matrix
    computeMatrix();

    glPushMatrix();
	glMultMatrixf(m_transformation.getData());
    m_mesh->render();
    glPopMatrix();
}

bool SpaceCraft::hasMesh() const
{
    return m_mesh != nullptr;
}


//TODO: Feinschliff für Hitboxen
bool SpaceCraft::collideShip(Vector<float> vec, float r)
{
    Vector<float> axis = getXAxis();
    Vector<float> pos = getPosition();
    Vector<float> dist = pos - vec;
    float distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    //cerr << "distance :" << distance;
    if(distance <= pow(r + 50, 2))
    {
        cerr << "test";
        return true;
    }
    pos = pos + axis * 90;
    dist = pos - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 40, 2))
    {
        cerr << "test";
        return true;
    }
    pos = pos + axis * 70;
    dist = pos - vec;
    distance = pow(dist[0], 2) + pow(dist[1], 2) + pow(dist[2], 2);
    if(distance <= pow(r + 30, 2))
    {
        cerr << "test";
        return true;
    }


    return false;
}

SpaceCraft::~SpaceCraft()
{
 
}

} // namespace asteroids
