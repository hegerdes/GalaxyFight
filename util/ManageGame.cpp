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

ManageGame *ManageGame::instance = nullptr;

ManageGame::ManageGame(QObject *parent, PlanetChanges::Owner player_id, int planet_id) : QObject(parent), m_player_id(player_id), m_base(planet_id)
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

    //Die am Start verfügbaren Schiffe werden den Listen hinzugefügt
    for (int i = 1; i <= m_attackSpaceCraft_number;i++) {
        std::shared_ptr<attackspacecraft> attackSpaceCraft = std::make_shared<attackspacecraft>(i, planet_id);
        attackSpaceCraft->m_next_position = planet_id;
        attackSpaceCraft->m_change_position = true;
        attackSpaceCraft->m_owner = m_player_id;
        m_attackSpaceCraftslist.push_back(attackSpaceCraft);
    }
    for (int i = 1; i <= m_transportSpaceCraft_number;i++) {
        std::shared_ptr<transportspacecraft> transportSpaceCraft = std::make_shared<transportspacecraft>(i,planet_id);
        transportSpaceCraft->m_owner = m_player_id;
        m_transportSpaceCraftslist.push_back(transportSpaceCraft);
    }

    updateStats();
}

ManageGame* ManageGame::initialize_player(PlanetChanges::Owner player_id, int planet_id)
{
   ManageGame::instance = new ManageGame(nullptr,player_id,planet_id);

   return ManageGame::instance;
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
    if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
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
        else if(m_planets.at((unsigned long)planet_id)->getFactorys() != 0)
        {
            emit already_exist();
        }else
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
    if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
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
    if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
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

            m_attackspacecraft_id += 1;
            std::shared_ptr<attackspacecraft> attackSpaceCraft = std::make_shared<attackspacecraft>(m_attackspacecraft_id, planet_id);
            attackSpaceCraft->m_next_position = planet_id;
            attackSpaceCraft->m_change_position = true;
            attackSpaceCraft->m_owner = m_player_id;
            m_attackSpaceCraftslist.push_back(attackSpaceCraft);

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
    if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
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

            m_transportSpaceCraft_number += 1;
            std::shared_ptr<transportspacecraft> transportSpaceCraft = std::make_shared<transportspacecraft>(m_transportSpaceCraft_number, planet_id);
            transportSpaceCraft->m_owner = m_player_id;
            m_transportSpaceCraftslist.push_back(transportSpaceCraft);

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

void ManageGame::change_Fighter_position(int new_position, int attackSpaceCraft_id){
    for (auto i = m_attackSpaceCraftslist.begin(); i != m_attackSpaceCraftslist.end(); i++) {
        if ((*i)->m_id == attackSpaceCraft_id){
            (*i)->m_next_position = new_position;
            (*i)->m_change_position = true;
            break;
        }
    }
}

void ManageGame::change_transport_route(int planet_id, int transportSpaceCraft_id){
    //checken ob der planet mit der neuen route auch dem player gehört
    if((m_planets[(unsigned long)planet_id])->getOwner() == m_player_id){
        for (auto i = m_transportSpaceCraftslist.begin(); i != m_transportSpaceCraftslist.end(); i++) {
            if ((*i)->m_id == transportSpaceCraft_id){

                (*i)->m_route_iterator = (*i)->m_tmp_route.begin();
                (*i)->m_to_new_route = true;
            }
        }
    }else {
        //falscher owner
    }
}

void ManageGame::next_round()
{
    //Convert to list to send
    m_round_changes_list = get_PlanetCangeList();

    //Clear change_map for next round
    m_round_changes_map.clear();

    //Raumschiffrouten aktualisieren
    updateSpaceCrafts();

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

void ManageGame::updateSpaceCrafts()
{
    //fighterrouten aktualisieren
    for (auto i = m_attackSpaceCraftslist.begin();i != m_attackSpaceCraftslist.end(); i++) {
        if((*i)->m_change_position == true){
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_change_position = false;
        }
    }

    //transportrouten aktualisieren
    for(auto i = m_transportSpaceCraftslist.begin(); i != m_transportSpaceCraftslist.end(); i ++){
        if((*i)->m_to_new_route == true){
            if((*i)->m_route_iterator == (*i)->m_tmp_route.end()){
                (*i)->m_route_iterator = (*i)->m_current_route.begin();
                (*i)->m_to_base = true;
                (*i)->m_to_new_route = false;
                (*i)->m_position = *((*i)->m_route_iterator++);
                (*i)->m_next_position = *((*i)->m_route_iterator);
            }
            (*i)->m_position = *((*i)->m_route_iterator++);
            (*i)->m_next_position = *((*i)->m_route_iterator);
        }
        if((*i)->m_to_base == true){
            if((*i)->m_route_iterator == (*i)->m_current_route.end()){
                (*i)->m_to_base = false;
                (*i)->m_to_mine = true;
                (*i)->m_position = *((*i)->m_route_iterator--);
                (*i)->m_next_position = *((*i)->m_route_iterator);
            }else {
                (*i)->m_position = *((*i)->m_route_iterator++);
                (*i)->m_next_position = *((*i)->m_route_iterator);
            }
        }
        if((*i)->m_to_mine == true){
            if((*i)->m_route_iterator == (*i)->m_current_route.begin()){
                (*i)->m_to_base = true;
                (*i)->m_to_mine = false;
                (*i)->m_position = *((*i)->m_route_iterator++);
                (*i)->m_next_position = *((*i)->m_route_iterator);

            }else {
                (*i)->m_position = *((*i)->m_route_iterator--);
                (*i)->m_next_position = *((*i)->m_route_iterator);
            }
        }
    }
}

ManageGame::~ManageGame()
{
    //TODO
}
} // namespace asteroids
