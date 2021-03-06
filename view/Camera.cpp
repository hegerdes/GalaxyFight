/*
 *  Camera.cpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

#include "view/Camera.hpp"

namespace asteroids
{

Camera::Camera():m_position(0, 0, 0), m_firstPerson(false), m_up(0, 0, 1), m_l(0, 0, 0)
{
}

Camera::Camera(const Vector3f& pos):m_firstPerson(false)
{
    /* Init pos */
    for(int i = 0; i < 3; i++)
    {
        m_position[i] = pos[i];        
    }

    
    /* Init look up vector */
    m_up[0]  = 0.0f;
    m_up[1]  = 0.0f;
    m_up[2]  = 1.0f;
    
    /* Init look at vector */
    m_l[0]   = 0.0f;
    m_l[1]   = 0.0f;
    m_l[2]   = 0.0f;
}


void Camera::follow(asteroids::SpaceCraft::Ptr s)
{
    if(m_firstPerson)
    {
        m_position = s->getPosition();
    }
    else
    {
       m_position = s->getPosition() + s->getZAxis() * 200 + s->getXAxis() * 300;         
    }  
    m_up = s->getZAxis();
    m_l = s->getPosition() - s->getXAxis() * 300;
}

void Camera::apply()
{
    /* Calc look at vector based on rotation state */
    // m_l[0] =  m_initial[0] + m_trans[0] + sin(m_rot[1]);
    // m_l[2] = -m_initial[2] - m_trans[2] - cos(m_rot[1]);
    // m_l[1] =  m_initial[1] + m_trans[1] + sin(m_rot[0]);

    /* Clear matrix stack */
    glLoadIdentity();
    
    /* Apply transformation */
    gluLookAt(m_position[0], m_position[1], m_position[2],
              m_l[0], m_l[1], m_l[2],
              m_up[0], m_up[1], m_up[2]);
}


} // namespace asteroids
