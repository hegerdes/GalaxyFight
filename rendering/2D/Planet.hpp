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
#include "settings/init_file.h"

namespace asteroids
{

class Planet
{
  public:
    using Ptr = std::shared_ptr<Planet>;

    /**
     * @brief Construct a new Planet number
     * 
     * @param name The name of the planet
     * @param pos Vector with position
     */
    Planet(int id, std::string name, Vector3f pos);

    /**
     * @brief Updates the planet with all changes that the user made
     * 
     * @param updates Update-file with the information to update
     * @return int 1 if succsess
     */
    int updatePlanet(PlanetChanges::Ptr updates);

    /**
     * @brief Print a Planet to std::cout
     * 
     */
    void printPlanet();

    /**
     * @brief Getter for the planet-name
     * 
     * @return std::string Name of the planet
     */
    inline std::string getname(){return m_planet_name;}

    /**
     * @brief Getter for ID
     * 
     * @return int ID of planet
     */
    inline int getID(){return m_id;}

    /**
     * @brief Get the Ore number
     * 
     * @return int Number of ore on planet
     */
    inline int getOre(){return m_num_of_ore;}
    
    /**
     * @brief Get the Factorys number
     * 
     * @return int Number of factorys on planet
     */
    inline int getFactorys(){return m_num_factory;}

    /**
     * @brief Get the Fighter number
     * 
     * @return int Number of fighter on planet
     */
    inline int getFighter(){return m_num_fighters;}

    /**
     * @brief Get the Transporter number
     * 
     * @return int Number of transporter on planet
     */
    inline int getTransporter(){return m_num_transporter;}

    /**
     * @brief Get the Mine number
     * 
     * @return int Number of mines on planet
     */
    inline int getMine(){return m_num_mine;}

    /**
     * @brief Get the Stored Ore number
     * 
     * @return int Number of stored ore on planet
     */

    inline int getStoredOre(){return m_stored_ore;}

    /**
     * @brief Get the Owner number
     * 
     * @return PlanetChanges::Owner The owner of the planet 
     */
    inline PlanetChanges::Owner getOwner(){return m_owned;}

    /**
     * @brief Get the Pos number
     * 
     * @return Vector3f& Position of the planet
     */
    inline Vector3f& getPos(){return m_planet_pos;}
        

  private:

    /**
     * @brief initialize all values except m_id, name and position
     * 
     * @return int 1 if succsess
     */
    int init();

    //ID of the planet
    int m_id;

    //Name of the planet
    std::string m_planet_name;

    //Number of ore on planet
    int m_num_of_ore;

    //Number of stored ore
    int m_stored_ore;

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
    PlanetChanges::Owner m_owned;
    
    
};

}//asteroids

#endif
