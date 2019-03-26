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
#include "global_socket.h"

namespace asteroids
{

ManageGame *ManageGame::instance = nullptr;

ManageGame::ManageGame(QObject *parent) : QObject(parent)
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

    updateStats();
}

ManageGame *ManageGame::getinstance()
{
    if (!ManageGame::instance)
    {
        ManageGame::instance = new ManageGame;
    }
    return ManageGame::instance;
}

void ManageGame::build_factory(int planet_id)
{
    //Ceck if planet belongs to player
    //TODO chane PlanetChanges::UNASSIGN to m_player_id
    if (m_planets.at((unsigned long)planet_id)->getOwner() != PlanetChanges::UNASSIGN)
    {
        emit not_ur_planet();
    }
    else
    {
        //Ceck for ressources
        if (m_current_resource < COST_PER_SHIPYARD)
        {
            emit no_resources();
        }
        else
        {
            m_current_resource -= COST_PER_SHIPYARD;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if (search != m_round_changes_map.end())
            {
                search->second->setFactorys(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
                change->setFactorys(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit updateInfobar();
        }
    }
}

void ManageGame::build_mine(int planet_id)
{
    //Ceck if planet belongs to player
    //TODO chane PlanetChanges::UNASSIGN to m_player_id
    if (m_planets.at((unsigned long)planet_id)->getOwner() != PlanetChanges::UNASSIGN)
    {
        emit not_ur_planet();
    }
    else
    {
        //Ceck for ressources
        if (m_current_resource < COST_PER_MINE)
        {
            emit no_resources();
        }
        else
        {
            m_current_resource -= COST_PER_MINE;
            m_resource_per_time += RESOURCE_PER_MINE;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if (search != m_round_changes_map.end())
            {
                search->second->setMines(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
                change->setMines(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit updateInfobar();
        }
    }
}
void ManageGame::build_fighter(int planet_id)
{
    //Ceck if planet belongs to player
    //TODO chane PlanetChanges::UNASSIGN to m_player_id
    if (m_planets.at((unsigned long)planet_id)->getOwner() != PlanetChanges::UNASSIGN)
    {
        emit not_ur_planet();
    }
    else
    {
        //Ceck for ressources
        if (m_current_resource < COST_PER_ATTACKSPACECRAFT)
        {
            emit no_resources();
        }
        else
        {
            m_current_resource -= COST_PER_ATTACKSPACECRAFT;
            std::cout << "Test aus Mine" << std::endl;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if (search != m_round_changes_map.end())
            {
                search->second->setFighter(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
                change->setFighter(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit updateInfobar();
        }
    }
}
void ManageGame::build_transporter(int planet_id)
{
    //Ceck if planet belongs to player
    //TODO chane PlanetChanges::UNASSIGN to m_player_id
    if (m_planets.at((unsigned long)planet_id)->getOwner() != PlanetChanges::UNASSIGN)
    {
        emit not_ur_planet();
    }
    else
    {
        //Ceck for ressources
        if (m_current_resource < COST_PER_TRANSPORTSPACECRAFT)
        {
            emit no_resources();
        }
        else
        {
            m_current_resource -= COST_PER_TRANSPORTSPACECRAFT;

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if (search != m_round_changes_map.end())
            {
                search->second->setTransports(1);
            }
            else
            {
                PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
                change->setTransports(1);
                m_round_changes_map[planet_id] = change;
            }
            //update signal für die info bar
            emit updateInfobar();
        }
    }
}
void ManageGame::next_round()
{
    //Convert to list to send
    m_round_changes_list = get_PlanetCangeList();

    //Clear change_map for next round
    m_round_changes_map.clear();

    //TODO Sent List
    //inet network
    //sent changes
    //check for fights
    //init fights
    //end fights
    //get changes
    //Go through all receved changes

    for (auto it_c = m_round_changes_list.begin(); it_c != m_round_changes_list.end(); it_c++)
    {
        //Add ore change to round_changes. depends on planet_id and mines on this planet
        it_c->get()->setOre(m_planets.at((unsigned long)it_c->get()->getID())->getMine() * RESOURCE_PER_MINE);
        //Applay changes
        m_planets.at((unsigned long)it_c->get()->getID())->updatePlanet(*(it_c));
    }

    //TODO Next 2d-round
    updateStats();

    emit updateInfobar();
    // @ahaker send initpacket
    std::list<PlanetChanges> changes;
    PlanetChanges planetchanges(PlanetChanges::Owner::PLAYER1 ,1,1,1,1,1,1,0);
    PlanetChanges planetchangess(PlanetChanges::Owner::PLAYER1 ,2,1,1,1,1,1,0);
    changes.push_back(planetchanges);
    changes.push_back(planetchangess);
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << ", changes.size()" << changes.size() << "\n";
    client_global.SendPlanetChanges(changes.size(), changes);
    std::cerr << __LINE__ << "\n";
    client_global.wait_for_readData(-1);
    while(!client_global.init_received) {
        client_global.SendPlanetChanges(changes.size(), changes);
        client_global.wait_for_readData(-1);
        sleep(1);
    }
    std::cerr << __LINE__ << "\n";
    emit goto3DScene();
}

void ManageGame::end_game()
{
}

void ManageGame::updateStats()
{
    //Reset items
    m_attackSpaceCraft_number = 0;
    m_transportSpaceCraft_number = 0;
    m_resource_per_time = 0;
    m_global_mines = 0;

    //Go through all planets and update items
    for (const auto &planet : m_planets)
    {
        //TODO Remove test
        //planet->printPlanet();

        //Update only if the planet is owned by player
        //if (planet->getOwner() == m_player_id)
        //TODO vergleichen mit m_player_id
        if (planet->getOwner() == PlanetChanges::UNASSIGN)
        {
            m_attackSpaceCraft_number += planet->getFighter();
            m_transportSpaceCraft_number += planet->getTransporter();
            m_global_mines += planet->getMine();
        }
    }
    //Update current resource and resource_per time
    m_resource_per_time = m_global_mines * RESOURCE_PER_MINE;
    m_current_resource += m_resource_per_time;

    emit updateInfobar();
}

std::list<PlanetChanges::Ptr> &ManageGame::get_PlanetCangeList()
{
    m_round_changes_list.clear();
    for (auto it = m_round_changes_map.begin(); it != m_round_changes_map.end(); it++)
    {
        m_round_changes_list.push_back(it->second);
        it->second->print();
    }
    return m_round_changes_list;
}

ManageGame::~ManageGame()
{
    //TODO
}
} // namespace asteroids
