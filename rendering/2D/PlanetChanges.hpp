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

#include <memory>


namespace asteroids
{

class PlanetChanges
{

  public:

    using Ptr = std::shared_ptr<PlanetChanges>;

    enum Owner{UNASSIGN, PLAYER1, PLAYER2};

  /**
   * @brief Construct a new Planet Changes object
   * 
   * @param id 
   */
  PlanetChanges(int id)
  {
    id = m_id;
  }

  PlanetChanges(Owner own, int id, int ore, int factory, int mine, int fighters, int transporter, bool attack)
  {
      m_owned = own;
      m_id = id;
      m_num_of_ore = ore;
      m_num_factory = factory;
      m_num_mine = mine;
      m_num_fighters = fighters;
      m_num_transporter = transporter;
      m_attack_planet = attack;
  }

  inline int getID(){return m_id;}

  inline int getFactorys(){return m_num_factory;}

  inline int getMines(){return m_num_mine;}

  inline int getOre(){return m_num_of_ore;}

  inline int getFighter(){return m_num_fighters;}

  inline int getTransports(){return m_num_transporter;}

  inline bool getInitFight(){return m_attack_planet;}

  inline Owner geOwner(){return m_owned;}

  inline void setFactorys(int factorys){m_num_factory = factorys;}

  inline void setMines(int mines){m_num_mine = mines;}

  inline void setOre(int ore){m_num_of_ore = ore;}

  inline void setFighter(int fighter){m_num_fighters = fighter;}

  inline void setTransports(int transporter){m_num_transporter = transporter;}

  inline void setInitFight(bool startFight){m_attack_planet = startFight;}

  inline void setOwner(Owner o){m_owned = o;}

  private:

    //ID of the planet
    int m_id;

    //Number of ore on planet
    int m_num_of_ore;

    //Number of factorys
    int m_num_factory;

    //Number of mines
    int m_num_mine;

    //Number of fighters on planet
    int m_num_fighters;

    //Number of transporters on planet
    int m_num_transporter;

    //if player wants to start a fight
    bool m_attack_planet;

    //Who owns the planet
    Owner m_owned;
  
};

}//asteroids

#endif
