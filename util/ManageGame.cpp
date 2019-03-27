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

ManageGame::ManageGame(QObject *parent) : QObject(parent), m_initialised(false)
{
    //Read in defaults
    m_current_resource = START_RESOURCE;
    m_resource_per_time = START_RESOURCE_PER_TIME;
    m_attackSpaceCraft_number = START_ATTACKSPACECRAFT_NUMBER;
    m_transportSpaceCraft_number = START_TRANSPORTSPACECRAFT_NUMBER;
    m_attackspacecraft_id = m_attackSpaceCraft_number;

    //Init of members
    MapFactory& b = MapFactory::getinstance();
    m_planetmap = b.getMap("models/01.map");
    m_planets = m_planetmap->getPlanets();
    m_round_changes_map = std::map<int,PlanetChanges::Ptr>();
    m_round_changes_list = std::list<PlanetChanges::Ptr>();
    m_attackSpaceCraftslist = std::list<Fighter>();
    m_transportSpaceCraftslist = std::list<Transporter>();


    updateStats();
}

void ManageGame::build_factory(int planet_id)
{
    if(m_initialised)
    {
        //Ceck if planet belongs to player
        if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
        {
            emit not_ur_planet();
        }
        else
        {
            //Ceck for ressources
            if (m_current_resource < COST_PER_SHIPYARD)
            {
                emit no_resources(0);
            }
            else if(m_planets.at((unsigned long)planet_id)->getFactorys() != 0||
                    m_round_changes_map.find(planet_id)->second->getFactorys() == 1)
            {
                emit already_exist();
            }else
            {
                m_current_resource -= COST_PER_SHIPYARD;

                //Check for alrady existing change for this planet
                auto search = m_round_changes_map.find(planet_id);
                if (search == m_round_changes_map.end())
                {
                    m_round_changes_map[planet_id] = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
                }
                m_round_changes_map[planet_id]->setFactorys(1);
                //update signal für die info bar
                emit updateInfobar();
            }
        }
    }
    else
    {
        throwError();
    }
}
void ManageGame::build_mine(int planet_id)
{
    if(m_initialised)
    {
        //Ceck if planet belongs to player
        if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
        {
            emit not_ur_planet();
        }
        else
        {
            //Ceck for ressources
            if (m_current_resource < COST_PER_MINE)
            {
                emit no_resources(0);
            }
            else
            {
                m_current_resource -= COST_PER_MINE;
                m_resource_per_time += RESOURCE_PER_MINE;

                //Check for alrady existing change for this planet
                auto search = m_round_changes_map.find(planet_id);
                if (search == m_round_changes_map.end())
                {
                    m_round_changes_map[planet_id] = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
                }
                m_round_changes_map[planet_id]->setMines(1);
                //update signal für die info bar
                emit updateInfobar();
            }
        }
    }
    else
    {
        throwError();
    }
}
void ManageGame::build_fighter(int planet_id)
{
    if(m_initialised)
    {
        //Ceck if planet belongs to player
        if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
        {
            emit not_ur_planet();
        }
        else if(m_current_resource < COST_PER_ATTACKSPACECRAFT)
        {
            emit no_resources(0);
        }
        else if(m_planets.at((unsigned long)planet_id)->getFactorys() == 0)
        {
            emit noFactory();
        }
        else
        {

            m_current_resource -= COST_PER_ATTACKSPACECRAFT;

            m_attackspacecraft_id += 1;
            m_attackSpaceCraft_number +=1;
            Fighter attackSpaceCraft = std::make_shared<attackspacecraft>(m_attackspacecraft_id, planet_id);
            attackSpaceCraft->m_next_position = planet_id;
            attackSpaceCraft->m_change_position = true;
            attackSpaceCraft->m_owner = m_player_id;
            m_attackSpaceCraftslist.push_back(attackSpaceCraft);

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if (search == m_round_changes_map.end())
            {
                m_round_changes_map[planet_id] = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
            }

            m_round_changes_map[planet_id]->setFighter(1);

            //update signal für die info bar
            emit updateInfobar();

        }
    }
    else
    {
        throwError();
    }
}
void ManageGame::build_transporter(int planet_id)
{
    if(m_initialised)
    {
        //Ceck if planet belongs to player
        if (m_planets.at((unsigned long)planet_id)->getOwner() != m_player_id)
        {
            emit not_ur_planet();
        }
        else if(m_current_resource < COST_PER_TRANSPORTSPACECRAFT)
        {
           emit no_resources(0);
        }
        else if(m_planets.at((unsigned long)planet_id)->getFactorys() == 0)
        {
            emit noFactory();
        }
        else
        {
            m_current_resource -= COST_PER_TRANSPORTSPACECRAFT;

            m_transportSpaceCraft_number += 1;
            Transporter transportSpaceCraft = std::make_shared<transportspacecraft>(m_transportSpaceCraft_number, planet_id);
            transportSpaceCraft->m_owner = m_player_id;
            m_transportSpaceCraftslist.push_back(transportSpaceCraft);

            //Check for alrady existing change for this planet
            auto search = m_round_changes_map.find(planet_id);
            if (search == m_round_changes_map.end())
            {
                m_round_changes_map[planet_id] = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
            }
            m_round_changes_map[planet_id]->setTransports(1);

            //update signal für die info bar
            emit updateInfobar();
        }

    }
    else
    {
        throwError();
    }
}

void ManageGame::destroy_fighter(int fighter_id)
{
    if(m_initialised)
    {
        for (auto i = m_attackSpaceCraftslist.begin(); i != m_attackSpaceCraftslist.end(); i++) {
                if ((*i)->m_id == fighter_id){
                    i = m_attackSpaceCraftslist.erase(i);
                    m_attackSpaceCraft_number -= 1;
                    break;
                }
        }

    }else
    {
        throwError();
    }

}

void ManageGame::change_Fighter_position(int new_position, int attackSpaceCraft_id)
{
    if(m_initialised)
    {
        for (auto i = m_attackSpaceCraftslist.begin(); i != m_attackSpaceCraftslist.end(); i++)
        {
            if ((*i)->m_id == attackSpaceCraft_id){
                auto tmp = m_planetmap->getPath((*i)->m_position, new_position);
                if(tmp.size() == 2)
                {
                    (*i)->m_next_position = new_position;
                    (*i)->m_change_position = true;

                    if(m_planets[new_position]->getOwner() == PlanetChanges::UNASSIGN)
                    {
                        //Check for alrady existing change for this planet
                        auto search = m_round_changes_map.find(new_position);
                        if (search == m_round_changes_map.end())
                        {
                            m_round_changes_map[new_position] = std::make_shared<PlanetChanges>(PlanetChanges(new_position));
                        }
                        m_round_changes_map[new_position]->setFighter(1);
                        m_round_changes_map[new_position]->setOwner(m_player_id);

                    }else if (m_planets[new_position]->getOwner() != m_player_id)
                    {
                        //Check for alrady existing change for this planet
                        auto search = m_round_changes_map.find(new_position);
                        if (search == m_round_changes_map.end())
                        {
                            m_round_changes_map[new_position] = std::make_shared<PlanetChanges>(PlanetChanges(new_position));
                        }
                        m_round_changes_map[new_position]->setFighter(1);
                        m_round_changes_map[new_position]->setInitFight(true);
                    }
                    break;
                }
                else
                {
                    emit changeRouteError();
                }

            }
        }
    }
    else
    {
        throwError();
    }
}

void ManageGame::change_transport_route(int planet_id, int transportSpaceCraft_id)
{
    if(m_initialised)
    {
        //checken ob der planet mit der neuen route auch dem player gehört
        if((m_planets[(unsigned long)planet_id])->getOwner() == m_player_id)
        {
            for (auto i = m_transportSpaceCraftslist.begin(); i != m_transportSpaceCraftslist.end(); i++)
            {
                if ((*i)->m_id == transportSpaceCraft_id){
                    (*i)->m_tmp_route.clear();
                    (*i)->m_current_route.clear();
                    (*i)->m_tmp_route = m_planetmap->getPath((*i)->m_position, planet_id);
                    (*i)->m_current_route = m_planetmap->getPath(planet_id, m_base);
                    (*i)->m_route_iterator = (*i)->m_tmp_route.begin();
                    (*i)->m_to_new_route = true;
                    (*i)->m_to_base = false;
                    (*i)->m_to_mine = false;
                }
            }
        }
        else
        {
            //falscher owner
        }
    }
    else
    {
        throwError();
    }
}

void ManageGame::next_round()
{
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    if(m_initialised)
    {
        //Update all Ore and Stored ore on PlayerChanges
        for (auto it_p = m_planets.begin(); it_p != m_planets.end(); ++it_p)
        {
            if((*it_p)->getOwner() == m_player_id)
            {
                if((*it_p)->getOre() >= RESOURCE_PER_MINE)
                {
                    int tmp = (*it_p)->getID();
                    auto search = m_round_changes_map.find(tmp);
                    if (search == m_round_changes_map.end())
                    {
                        //Add in not found
                        m_round_changes_map[tmp] = std::make_shared<PlanetChanges>(PlanetChanges(tmp));
                    }
                    m_round_changes_map[tmp]->setOre( -1 * (*it_p)->getMine() * RESOURCE_PER_MINE);
                    m_round_changes_map[tmp]->setStoredOre((*it_p)->getMine() * RESOURCE_PER_MINE);
                }
            }
        }

        //Convert to list to send
        m_round_changes_list = get_PlanetCangeList();

        //Clear change_map for next round
        m_round_changes_map.clear();

        //Raumschiffrouten aktualisieren
        updateSpaceCrafts();

        emit stopTimer();

        //TODO Sent List
        //inet network
        //sent changes
        //check for fights
        //init fights
        //end fights
        //get changes
        //Go through all receved changes

        // emit resetTimer();
        for (auto it_c = m_round_changes_list.begin(); it_c != m_round_changes_list.end(); it_c++)
        {
            //Applay changes
            m_planets.at((unsigned long)it_c->get()->getID())->updatePlanet(*(it_c));
        }

        //TODO Next 2d-round
        updateStats();

        emit updateScene();
        std::cerr << __LINE__ << " updateScene called " << __LINE__ << std::endl;
        emit updateInfobar();
    }

    emit updateInfobar();
    // @ahaker send initpacket
    std::list<PlanetChanges> changes;
    /*
    for(auto x : m_round_changes_map)
    {
        changes.push_back(x.first);
    }
    */
    PlanetChanges planetchanges(PlanetChanges::Owner::UNASSIGN ,3,1,1,1,1,1,0,1);
    changes.push_back(planetchanges);
    //PlanetChanges planetchangess(PlanetChanges::Owner::UNASSIGN ,1,1,1,1,1,1,0,1);
    //changes.push_back(planetchangess);

    client_global.m_planet_changes_received = false;
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << ", changes.size()" << changes.size() << "\n";
    client_global.SendPlanetChanges(changes.size(), changes);
    if(!changes.empty())
    {
        changes.erase(changes.begin());
    }
    client_global.readData();
    if(client_global.init_received){
        std::cerr << __LINE__ << "----------------------------------------\n";
        emit goto3DScene();
    }else if (client_global.m_planet_changes_received) {
        std::cerr << __LINE__ << "----------------------------------------\n";
        //apply changes
    }
    //client_global.init_received = false;
    while(!client_global.init_received || !client_global.m_planet_changes_received) {
        client_global.rerequest_planet_changes();
        client_global.wait_for_readData(-1);
        if(client_global.init_received){
            emit goto3DScene();
            break;
        }else if (client_global.m_planet_changes_received) {
        //apply changes
            break;
        }
        sleep(1);
        std::cerr << __LINE__ << "\n";
    }
    //client_global.send_reset_planet_changes(); @ahaker beim merge wegefallen
    //client_global.m_planet_changes_received = false; @ahaker beim merge wegefallen
    //client_global.wait_for_readData(100);
    std::cerr << __LINE__ << "\n";
}

void ManageGame::end_game()
{
}

void ManageGame::updateStats()
{
    //save for compare
    int tmp = m_global_mines;

    //Reset items
    m_resource_per_time = 0;
    m_global_mines = 0;

    //Go through all planets and update items
    for (const auto &planet : m_planets)
    {
        //Update only if the planet is owned by player
        if (planet->getOwner() == m_player_id)
        {
            if(planet->getOre() >= 0)
            {
                m_global_mines += planet->getMine();
            }
        }

    }
    if(tmp > m_global_mines)
    {
        emit no_resources(tmp - m_global_mines);
    }
    //Update current resource and resource_per time
    m_resource_per_time = m_global_mines * RESOURCE_PER_MINE;

    emit updateInfobar();
}

std::list<PlanetChanges::Ptr> &ManageGame::get_PlanetCangeList()
{
    m_round_changes_list.clear();
    for (auto it = m_round_changes_map.begin(); it != m_round_changes_map.end(); it++)
    {
        m_round_changes_list.push_back(it->second);
        //For Debug
        //it->second->print();
    }
    return m_round_changes_list;
}

void ManageGame::updateSpaceCrafts()
{
    //fighterrouten aktualisieren
    for (auto i = m_attackSpaceCraftslist.begin();i != m_attackSpaceCraftslist.end(); i++) {
        if((*i)->m_change_position){
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_change_position = false;
        }
    }

    //transportrouten aktualisieren
    for(auto i = m_transportSpaceCraftslist.begin(); i != m_transportSpaceCraftslist.end(); i ++){
        if((*i)->m_to_new_route){

            if((*i)->m_route_iterator == (*i)->m_tmp_route.end()){
                (*i)->m_route_iterator = (*i)->m_current_route.begin();
                (*i)->m_to_base = true;
                (*i)->m_to_mine = false;
                (*i)->m_to_new_route = false;
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);

                //transporter lädt erz
                (*i)->m_ore = transporter_stored_ore((*i)->m_position);

            }else
            {
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);
            }
        }

        if((*i)->m_to_base){
            if((*i)->m_route_iterator == (*i)->m_current_route.end()){
                (*i)->m_to_base = false;
                (*i)->m_to_mine = true;
                (*i)->m_to_new_route = false;

                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_route_iterator--;
                (*i)->m_next_position = *((*i)->m_route_iterator--);

                m_current_resource += (*i)->m_ore;
                (*i)->m_ore = 0;

                emit updateInfobar();

            }else {
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);


            }
        }

        if((*i)->m_to_mine){
            if((*i)->m_route_iterator == (*i)->m_current_route.begin()){
                (*i)->m_to_base = true;
                (*i)->m_to_mine = false;
                (*i)->m_to_new_route= false;

                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);

                //transporter lädt erz
                (*i)->m_ore = transporter_stored_ore((*i)->m_next_position);

            }else {
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator--);

            }
        }
    }
}

int ManageGame::transporter_stored_ore(int transporter_position)
{
    int tmp;
    //Check for alrady existing change for this planet
    auto search = m_round_changes_map.find(transporter_position);
    if (search != m_round_changes_map.end())
    {
        if(m_planets[transporter_position]->getStoredOre() >= 200)
        {
            search->second->setStoredOre(-200);
            tmp = 200;

        }else if(m_planets[transporter_position]->getStoredOre() != 0)
        {
            search->second->setStoredOre(-(m_planets[transporter_position]->getStoredOre()));
            tmp = m_planets[transporter_position]->getStoredOre();
        }
    }
    else
    {
        PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(PlanetChanges(transporter_position));

        if(m_planets[transporter_position]->getStoredOre() >= 200)
        {
            change->setStoredOre(-200);
            tmp = 200;


        }else if(m_planets[transporter_position]->getStoredOre() != 0)
        {
            change->setStoredOre(-(m_planets[transporter_position]->getStoredOre()));
            tmp = m_planets[transporter_position]->getStoredOre();
        }

        m_round_changes_map[transporter_position] = change;
    }
    return tmp;
}

ManageGame *ManageGame::getinstance()
{
    if(!ManageGame::instance)
    {
        ManageGame::instance = new ManageGame();
    }
    return ManageGame::instance;
}

void ManageGame::initialize_player(PlanetChanges::Owner player_id, int planet_id)
{
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    if(!m_initialised)
    {
        m_player_id = player_id;
        m_base = planet_id;
		std::cerr << "\t" << __LINE__ << ", m_player_id: " << m_player_id << ", m_base: " << m_base << "\n";
        //TODO For DEBUG @ahaker
        //m_player_id = PlanetChanges::UNASSIGN;

        //set Satrt planet Owner
        //This is HQ
         PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
         change->setOwner(m_player_id);
         m_planets.at((unsigned long)planet_id)->updatePlanet(change);

    std::cerr << __LINE__ << "\n";
        //Die am Start verfügbaren Schiffe werden den Listen hinzugefügt
        for (int i = 0; i < m_attackSpaceCraft_number;i++)
        {
            Fighter attackSpaceCraft = std::make_shared<attackspacecraft>(attackspacecraft(i, planet_id));
            attackSpaceCraft->m_next_position = planet_id;
            attackSpaceCraft->m_change_position = true;
            attackSpaceCraft->m_owner = m_player_id;
            m_attackSpaceCraftslist.push_back(attackSpaceCraft);
        }
        for (int i = 0; i < m_transportSpaceCraft_number;i++)
        {
            Transporter transportSpaceCraft = std::make_shared<transportspacecraft>(transportspacecraft(i, planet_id));
            transportSpaceCraft->m_owner = m_player_id;
            m_transportSpaceCraftslist.push_back(transportSpaceCraft);
        }
        m_initialised = true;
        emit updateScene();
    }
    else
    {
        //throw std::logic_error("Alrady initialized");
    }
}

void ManageGame::throwError()
{
    throw std::logic_error("Not initialized");
}

ManageGame::~ManageGame()
{
    if(!ManageGame::instance)
    {
        delete ManageGame::instance;
    }
}
} // namespace asteroids
