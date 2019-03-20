/**
 * @file PlanetChanges.hpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief Holds all changes that can hapen to one planet.
 *        Will be used to sync gobal map between players.
 * @version 0.1
 * @date 2019-03-20
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef PLANETCHANGES_HPP
#define PLANETCHANGES_HPP

#include "Planet.hpp"

namespace asteroids
{

class PlanetChanges
{

  public:

  inline int getID(){return m_id;}

  inline int getFactorys(){return m_num_factory;}

  inline int getMines(){return m_num_mine;}

  inline int getOre(){return m_num_of_ore;}

  inline int getFighter(){return m_num_fighters;}

  inline int getTransports(){return m_num_transporter;}

  inline void setID(int id){m_id = id;}

  inline void setFactorys(int factorys){m_num_factory = factorys;}

  inline void setMines(int mines){m_num_mine = mines;}

  inline void setOre(int ore){m_num_of_ore = ore;}

  inline void setFighter(int fighter){m_num_fighters = fighter;}

  inline void setTransports(int transporter){m_num_transporter = transporter;}
    
  private:

    //ID of the planet
    int m_id;

    //Number of ore on planet
    unsigned int m_num_of_ore;

    //Number of factorys
    int m_num_factory;

    //Number of mines
    int m_num_mine;

    //Number of fighters on planet
    int m_num_fighters;

    //Number of transporters on planet
    int m_num_transporter;

    //Who owns the planet
    //Planet::Owner m_owned;
  
};

}//asteroids

#endif