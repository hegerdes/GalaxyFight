/**
 * @file ManageGame.cpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "ManageGame.hpp"

namespace asteroids
{

    ManageGame::ManageGame(QObject *parent, int base) : QObject(parent)
    {
        //Read in defaults
        m_current_resource = START_RESOURCE;
        m_resource_per_time = START_RESOURCE_PER_TIME;
        m_attackSpaceCraft_number = START_ATTACKSPACECRAFT_NUMBER;
        m_transportSpaceCraft_number = START_TRANSPORTSPACECRAFT_NUMBER;
        m_attackspacecraft_id = m_attackSpaceCraft_number;

        MapFactory& b = MapFactory::getinstance();
        m_planetmap = b.getMap("models/01.map");
        m_planets = m_planetmap->getPlanets();
    }

    ManageGame& ManageGame::getinstance()
    {
        static ManageGame instance;
        return instance;
    }

    void ManageGame::build_factory(int planet_id)
    {
        if(m_current_resource >= COST_PER_SHIPYARD)
        {
            //Decrease current state
            m_current_resource -= COST_PER_SHIPYARD;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if(search != m_round_changes_map.end())
            {
                //Chane for this planet exists alrady
                search->second->setFactorys(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(planet_id);
                change->setFactorys(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit update();
        }else {
            //nicht genug resourcen vorhanden
            emit no_resources();
        }
    }

    void ManageGame::build_mine(int planet_id)
    {
        if(m_current_resource >= COST_PER_MINE)
        {
            m_current_resource -= COST_PER_MINE;
            m_resource_per_time += RESOURCE_PER_MINE;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if(search != m_round_changes_map.end())
            {
                //Chane for this planet exists alrady
                search->second->setMines(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(planet_id);
                change->setMines(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit update();
        }else {
            //nicht genug resourcen vorhanden
            emit no_resources();
        }
    }

    void ManageGame::build_fighter(int planet_id)
    {
        if(m_current_resource >= COST_PER_TRANSPORTSPACECRAFT)
        {
            m_current_resource -= COST_PER_TRANSPORTSPACECRAFT;
            m_resource_per_time += RESOURCE_PER_MINE;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if(search != m_round_changes_map.end())
            {
                //Chane for this planet exists alrady
                search->second->setFighter(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(planet_id);
                change->setFighter(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit update();
        }else {
            //nicht genug resourcen vorhanden
            emit no_resources();
        }
    }

    void ManageGame::build_transporter(int planet_id)
    {
        if(m_current_resource >= COST_PER_TRANSPORTSPACECRAFT)
        {
            m_current_resource -= COST_PER_TRANSPORTSPACECRAFT;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if(search != m_round_changes_map.end())
            {
                //Chane for this planet exists alrady
                search->second->setTransports(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(planet_id);
                change->setTransports(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit update();
        }else {
            //nicht genug resourcen vorhanden
            emit no_resources();
        }
    }
    void ManageGame::next_round()
    {
        //Go through all changes and decrease ore on planet in changes if mine exists and owner is player
        //inet network 
        //sent changes
        //chech for fights
        //init fights
        //end fights
        //get changes
        //apply changes
        //updateStats
    }
    void ManageGame::end_game()
    {

    }
    void ManageGame::updateStats()
    {
        //Reset items
        m_attackSpaceCraft_number = 0;
        m_transportSpaceCraft_number = 0;
        m_global_mines = 0;

        //Go through all planets and update items
        for (const auto& planet : m_planets) 
        {
            //Update only if the planet is owned by player
            if(planet->getOwner() == m_player_id)
            {
                m_attackSpaceCraft_number += planet->getFighter();
                m_transportSpaceCraft_number += planet->getTransporter();
                m_global_mines += planet->getMine();
            }

        }
        m_resource_per_time = m_global_mines * RESOURCE_PER_MINE;
        m_current_resource += m_resource_per_time;

        emit update();
    }

    ManageGame::~ManageGame()
    {
        //TODO
    }
}// asteroids