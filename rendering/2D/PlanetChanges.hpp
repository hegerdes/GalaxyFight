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
#include <stdio.h>
#include <iostream>

namespace asteroids
{

class PlanetChanges
{

  public:

    using Ptr = std::shared_ptr<PlanetChanges>;

    //The possible Owners
    enum Owner{UNASSIGN, PLAYER1, PLAYER2};

    /**
     * @brief Construct a new Planet Changes object
     * 
     * @param id ID of the change. Is equal to PlanetID
     */
    PlanetChanges(int id)
    {
        m_id = id;
        init();
    }

    /**
     * @brief Construct a new Planet Changes-object
     *        For network to build a new updated change to be applyed to Planet
     * 
     * 
     * @param own Who owns the planet now
     * @param id The planet-id
     * @param ore How much ore changes
     * @param factory How many new factorys
     * @param mine How many new mines
     * @param fighters How many new fighters 
     * @param transporter How many new transporters
     * @param attack If wants to attack a the planet
     * @param storedOre The change on the stored ore on planet
     */
    PlanetChanges(Owner own, int id, int ore, int factory, int mine, int fighters, int transporter, bool attack, int storedOre)
    {
        m_owned = own;
        m_id = id;
        m_num_of_ore = ore;
        m_num_factory = factory;
        m_num_mine = mine;
        m_num_fighters = fighters;
        m_num_transporter = transporter;
        m_attack_planet = attack;
        m_store_ore = storedOre;
    }   
    
    /**
     * @brief Get the ChangeID. Equal to PlanetID
     * 
     * @return int The planetID
     */
    inline int getID(){return m_id;}  

    /**
     * @brief Get the Factorys number
     * 
     * @return int Number of changed factorys
     */
    inline int getFactorys(){return m_num_factory;} 

    /**
     * @brief Get the Mines number
     * 
     * @return int Number of changed mines
     */
    inline int getMines(){return m_num_mine;}

    /**
     * @brief Get the Ore number
     * 
     * @return int Number of changed ore
     */
    inline int getOre(){return m_num_of_ore;}

    /**
     * @brief Get the Fighter number
     * 
     * @return int Number of changed fighters
     */
    inline int getFighter(){return m_num_fighters;}

    /**
     * @brief Get the Transports numbers
     * 
     * @return int Number of changed transporters
     */
    inline int getTransports(){return m_num_transporter;}

    /**
     * @brief Get the Stored Ore number
     * 
     * @return int Number of changed stored ore
     */
    inline int getStoredOre(){return m_store_ore;}  

    /**
     * @brief Get the Init Fight flag
     * 
     * @return true If server should init a 3D fight
     * @return false If no 3D fight initialized by Player
     */
    inline bool getInitFight(){return m_attack_planet;}

    /**
     * @brief Get the Owner number
     * 
     * @return Owner of the planet
     */
    inline Owner getOwner(){return m_owned;}

    /**
     * @brief Set the Factorys number
     * 
     * @param factorys Number of chaged factorys
     */
    inline void setFactorys(int factorys){m_num_factory += factorys;}

    /**
     * @brief Set the Mines number
     * 
     * @param mines Number of changed mines
     */
    inline void setMines(int mines){m_num_mine += mines;}

    /**
     * @brief Set the Ore number
     * 
     * @param ore Number of changed ore
     */
    inline void setOre(int ore){m_num_of_ore += ore;}  

    /**
     * @brief Set the Fighter number
     * 
     * @param fighter Number of changed fighter
     */
    inline void setFighter(int fighter){m_num_fighters += fighter;}

    /**
     * @brief Set the Transports number
     * 
     * @param transporter Number of changed transporter
     */
    inline void setTransports(int transporter){m_num_transporter += transporter;}

    /**
     * @brief Set the Stored Ore number
     * 
     * @param ore Number of changed stored ore
     */
    inline void setStoredOre(int ore){m_store_ore += ore;}

    /**
     * @brief Set the Init Fight
     * 
     * @param startFight If Player wants to fight the planet of other player
     */
    inline void setInitFight(bool startFight){m_attack_planet = startFight;}

    /**
     * @brief Set the Owner object
     * 
     * @param o Owner of the planet
     */
    inline void setOwner(Owner o){m_owned = o;} 
    
    /**
     * @brief Print PlanetChight to std::cout
     * 
     */
    inline void print()
    {
        std::cout << "PlanetNr: " << m_id << " ChangeOwner: " << m_owned << " OreChange: "
        <<  m_num_of_ore << " FighterCange: " << m_num_fighters << " TransportChange: " 
        <<  m_num_transporter << " MineCange: " << m_num_mine << " FactoryChange: " 
        << m_num_factory << " StoredOreChange: " <<m_store_ore << " Attack: " 
        << m_attack_planet << std::endl;
    }

  private:

    /**
     * @brief Initilize all members with 0
     * 
     */
    void init()
    {
        m_owned = UNASSIGN;
        m_num_of_ore = 0;
        m_num_factory = 0;
        m_num_mine = 0;
        m_num_fighters = 0;
        m_num_transporter = 0;
        m_store_ore = 0; 
        m_attack_planet = false;
    }

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

    //Stored ore change
    int m_store_ore;

    //Number of transporters on planet
    int m_num_transporter;

    //if player wants to start a fight
    bool m_attack_planet;

    //Who owns the planet
    Owner m_owned;
  
};

}//asteroids

#endif
