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
    init();
}

int Planet::init()
{
    m_num_factory = 0;
    m_num_fighters = 0;
    m_num_mine = 0;
    m_num_transporter = 0;
    m_num_of_ore = 10;
    m_owned = UNASSIGN;
    return 1;
}

 int Planet::updatePlanet(PlanetChanges updates)
 {
    if(id != updates.getID()) return 1;
    m_num_factory = updates.getFactorys();
    m_num_fighters = updates.getFighter();
    m_num_mine = updates.getMines();
    m_num_transporter = updates.getTransports();
    m_num_of_ore = updates.getOre();
    m_owned = UNASSIGN;
    return 0;
 }

}//asteroids