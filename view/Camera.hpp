/*
 *  Camera.hpp
 *
 *  Created on: Nov. 04 2018
 *      Author: Thomas Wiemann
 *
 *  Copyright (c) 2018 Thomas Wiemann.
 *  Restricted usage. Licensed for participants of the course "The C++ Programming Language" only.
 *  No unauthorized distribution.
 */

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "math/Vector.hpp"
#include "../rendering/SpaceCraft.hpp"

#define PI 3.14159265
#define PH 1.57079632

namespace asteroids
{


/**
 * @brief Class to represent a virtual camera using gluLookAt
 * 
 */
class Camera
{
public:
    /**
     * @brief Enumeration to encode types of camera movements
     */
    enum CameraMovement 
    {
	    FORWARD,
	    BACKWARD,
	    LEFT,
	    RIGHT,
	    UP,
	    DOWN
    };

    /**
     * @brief Construct a new Camera object at (0, 0, 0) with
     *        upward orientation and lookAt at (0, 0, -1)
     * 
     */
    Camera();

    /**
     * @brief Construct a new Camera object with upward orientation
     *        and lookAt at (0, 0, -1)
     * 
     * @param position      Initial position
     */
    Camera(const Vector3f& position);

    /**
     * @brief Moves the camera behind the given spacecraft
     * 
     * @param s The spacecraft to be followed
     */ 
     void follow(SpaceCraft::Ptr s);

    /**
     * @brief Calls gluLookAt with the internal parameters
     * 
     */
    void apply();

    /**
     * @brief Set the view state of the camera(first or third person)
     * 
     * @param firstPerson   The new state(first person = true, third person = false)
     */
    void setFirstPerson(const bool firstPerson) {m_firstPerson = firstPerson;}
    
private:
    /// View up vector
    Vector3f  m_up;

    /// Look at Vector
    Vector3f  m_l;

    /// Position of the camera
    Vector3f  m_position;

    /// Current viewing state
    bool    m_firstPerson;
};

} // namespace asteroids

#endif
