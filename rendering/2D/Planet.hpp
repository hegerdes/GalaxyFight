/**
 * @file Planet.hpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief  A Planet with all properties needed
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef PLANET_HPP
#define PLANET_HPP


#include <stdio.h>
#include <string>
#include <memory>
#include <math.h>

#include "PlanetChanges.hpp"
#include "../../math/Vector.hpp"

namespace asteroids
{

class Planet
{
  public:
    using Ptr = std::shared_ptr<Planet>;
    enum Owner{UNASSIGN, PLAYER1, PLAYER2};

    /**
     * @brief Construct a new Planet object
     * 
     * @param name The name of the planet
     * @param pos Vector with position
     */
    Planet(std::string name, Vector3f pos);

    /**
     * @brief initialize all values except id, name and position
     * 
     * @return int 1 if succsess
     */
    int init();

    /**
     * @brief Updates the planet with all changes that the user made
     * 
     * @param updates Update-file with the information to update
     * @return int 1 if succsess
     */
    int updatePlanet(PlanetChanges updates);

  private:

    //ID of the planet
    int id;

    //Name of the planet
    std::string m_planet_name;

    //Number of ore on planet
    unsigned int m_num_of_ore;

    //Position in 3D space
    Vector3f m_planet_pos;

    //Number of factorys
    int m_num_factory;

    //Number of mines
    int m_num_mine;

    //Number of fighters on planet
    int m_num_fighters;

    //Number of transporters on planet
    int m_num_transporter;

    //Who owns the planet
    Owner m_owned;
    
    
};

}//asteroids

#endif