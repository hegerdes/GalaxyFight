/**
 * @file Planet.cpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#include "Planet.hpp"


namespace asteroids
{

Planet::Planet(std::string name, Vector3f pos):m_planet_name(name), m_planet_pos(pos)
{
}

float Planet::dist(Vector3f pos)
{
    return 5.0f;
}

}//asteroids