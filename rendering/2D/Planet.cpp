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
    m_num_of_ore = 10000;
    m_stored_ore = 0;
    m_owned = PlanetChanges::UNASSIGN;
    return 1;
}

 int Planet::updatePlanet(PlanetChanges::Ptr updates)
 {
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    if(m_id != updates->getID()) return 1;
    std::cerr << "\t" << __LINE__ << "\n";
    m_num_factory += updates->getFactorys();
    std::cerr << "\t" << __LINE__ << "\n";
    m_num_fighters += updates->getFighter();
    std::cerr << "\t" << __LINE__ << "\n";
    m_num_mine += updates->getMines();
    std::cerr << "\t" << __LINE__ << "\n";
    m_num_transporter += updates->getTransports();
    std::cerr << "\t" << __LINE__ << "\n";
    m_num_of_ore += updates->getOre();
    std::cerr << "\t" << __LINE__ << "\n";
    m_stored_ore += updates->getStoredOre();
    std::cerr << "\t" << __LINE__ << "\n";
    if(updates->getOwner() != PlanetChanges::UNASSIGN)
    {
    std::cerr << "\t" << __LINE__ << "\n";
        m_owned = updates->getOwner();
    }
    std::cerr << "\t" << __LINE__ << "\n";

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
