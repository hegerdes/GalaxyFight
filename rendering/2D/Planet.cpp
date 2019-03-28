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

Planet::Planet(int id, std::string name, Vector3f pos): m_id(id), m_planet_name(name), m_planet_pos(pos)
{
    init();
}

int Planet::init()
{
    m_num_factory = 0;
    m_num_fighters = 0;
    m_num_mine = 0;
    m_num_transporter = 0;
    m_num_of_ore = setting.value("Resourcen/Planetenerz").toInt();
    m_stored_ore = 0;
    m_owned = PlanetChanges::UNASSIGN;
    if( 0 == m_id){
        m_owned = PlanetChanges::PLAYER1;
    }
    return 1;
}

 int Planet::updatePlanet(PlanetChanges::Ptr updates)
 {
    //std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    if(m_id != updates->getID()) return 1;
    m_num_factory += updates->getFactorys();
    m_num_fighters += updates->getFighter();
    m_num_mine += updates->getMines();
    m_num_transporter += updates->getTransports();
    m_num_of_ore += updates->getOre();
    m_stored_ore += updates->getStoredOre();
    if(updates->getOwner() != PlanetChanges::UNASSIGN)
    {
        m_owned = updates->getOwner();
    }

    //update ore
    //m_num_of_ore -= m_num_mine * RESOURCE_PER_MINE;
    //m_stored_ore += m_num_mine * RESOURCE_PER_MINE;

    return 0;
 }

 void Planet::printPlanet()
 {
    std::cout << "PlanetNr: " << m_id << " Owner: " << m_owned << " Ore: " 
        <<  m_num_of_ore << " Fighter: " << m_num_fighters << " Transporter: " 
        <<  m_num_transporter << " Mine: " << m_num_mine << " Factory: " 
        << m_num_factory <<  "StoredOre: " << m_stored_ore << std::endl;
 }

}//asteroids
