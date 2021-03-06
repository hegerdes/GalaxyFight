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

ManageGame::ManageGame(QObject *parent) : QObject(parent), m_initialised(false), m_round_changes_map(), m_round_changes_list(),
    m_attackSpaceCraftslist(), m_transportSpaceCraftslist()
{
    //Read in defaults
    m_current_resource = setting.value("Resourcen/Starterz").toInt();
    m_resource_per_time = setting.value("Resourcen/Abbaurate").toInt();
    m_attackSpaceCraft_number = setting.value("Resourcen/AnfangsFighterAnzahl").toInt();
    m_transportSpaceCraft_number = setting.value("Resourcen/AnfangsTransportAnzahl").toInt();
    m_attackspacecraft_id = m_attackSpaceCraft_number;

    //get planet-map
    MapFactory& b = MapFactory::getinstance();
    m_planetmap = b.getMap(setting.value("Dateipfade/Map").toString().toStdString());
    m_planets = m_planetmap->getPlanets();


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
        else if(m_current_resource < setting.value("Resourcen/Werftkosten").toInt())
        {
            emit no_resources(0);
        }
        else if(m_planets.at((unsigned long)planet_id)->getFactorys() != 0||
                m_round_changes_map.find(planet_id)->second->getFactorys() == 1)
        {
            emit already_exist();
        }
        else
        {
            m_current_resource -= setting.value("Resourcen/Werftkosten").toInt();

            //Check for alrady existing change for this planet
            checkForChange(planet_id);
            m_round_changes_map[planet_id]->setFactorys(1);
            updateBase(-setting.value("Resourcen/Werftkosten").toInt());

            //update signal für die info bar
            emit updateInfobar();
        }
    }
    else {throwError();}
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
        else if(m_current_resource < setting.value("Resourcen/Mienekosten").toInt())
        {
            emit no_resources(0);
        }
        else
        {
            m_current_resource -= setting.value("Resourcen/Mienekosten").toInt();
            m_resource_per_time += setting.value("Resourcen/Abbaurate").toInt();

            //Check for alrady existing change for this planet
            checkForChange(planet_id);
            m_round_changes_map[planet_id]->setMines(1);
            updateBase(-setting.value("Resourcen/Mienekosten").toInt());

            //update signal für die info bar
            emit updateInfobar();
        }
    }
    else {throwError();}
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
        else if(m_current_resource < setting.value("Resourcen/Fighterkosten").toInt())
        {
            emit no_resources(0);
        }
        else if(m_planets.at((unsigned long)planet_id)->getFactorys() == 0)
        {
            emit noFactory();
        }
        else if(m_planets.at((unsigned long)planet_id)->getFighter() > 0 )
        {
            emit fighter_already_exist();
        }
        else
        {
            auto search = m_round_changes_map.find(planet_id);
            if(search == m_round_changes_map.end())
            {
                m_current_resource -= setting.value("Resourcen/Fighterkosten").toInt();

                m_attackspacecraft_id += 1;
                m_attackSpaceCraft_number +=1;
                Fighter attackSpaceCraft = std::make_shared<attackspacecraft>(m_attackspacecraft_id, planet_id, m_player_id);
                m_attackSpaceCraftslist.push_back(attackSpaceCraft);

                m_round_changes_map[planet_id] = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
                m_round_changes_map[planet_id]->setFighter(1);
                updateBase(-setting.value("Resourcen/Fighterkosten").toInt());

            }
            else
            {
                if(m_round_changes_map[planet_id]->getFighter() <= 0)
                {
                    m_current_resource -= setting.value("Resourcen/Fighterkosten").toInt();

                    m_attackspacecraft_id += 1;
                    m_attackSpaceCraft_number +=1;
                    Fighter attackSpaceCraft = std::make_shared<attackspacecraft>(m_attackspacecraft_id, planet_id, m_player_id);
                    m_attackSpaceCraftslist.push_back(attackSpaceCraft);

                    m_round_changes_map[planet_id]->setFighter(1);
                    updateBase(-setting.value("Resourcen/Fighterkosten").toInt());

                }
                else
                {
                    emit fighter_already_exist();
                }

            }
            //update signal für die info bar
            emit updateInfobar();

        }
    }
    else {throwError();}
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
        else if(m_current_resource < setting.value("Resourcen/Transporterkosten").toInt())
        {
           emit no_resources(0);
        }
        else if(m_planets.at((unsigned long)planet_id)->getFactorys() == 0)
        {
            emit noFactory();
        }
        else
        {
            m_current_resource -= setting.value("Resourcen/Transporterkosten").toInt();

            m_transportSpaceCraft_number += 1;
            Transporter transportSpaceCraft = std::make_shared<transportspacecraft>(m_transportSpaceCraft_number, planet_id, m_player_id);
            m_transportSpaceCraftslist.push_back(transportSpaceCraft);

            checkForChange(planet_id);
            m_round_changes_map[planet_id]->setTransports(1);
            updateBase(-setting.value("Resourcen/Transporterkosten").toInt());

            //update signal für die info bar
            emit updateInfobar();
        }

    }
    else {throwError();}
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

    }else {throwError();}
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

                    if(m_planets[new_position]->getOwner() == PlanetChanges::UNASSIGN)
                    {
                        checkForChange(new_position);
                        m_round_changes_map[new_position]->setFighter(1);
                        m_round_changes_map[new_position]->setOwner(m_player_id);

                        checkForChange((*i)->m_position);
                        m_round_changes_map[(*i)->m_position]->setFighter(-1);

                        if((*i)->m_change_position)
                        {
                            m_round_changes_map[(*i)->m_next_position]->setFighter(-1);
                            m_round_changes_map[(*i)->m_next_position]->setOwner(PlanetChanges::UNASSIGN);
                        }

                    }else if (m_planets[new_position]->getOwner() != m_player_id)
                    {
                        checkForChange(new_position);
                        m_round_changes_map[new_position]->setFighter(1);
                        m_round_changes_map[new_position]->setInitFight(true);

                        checkForChange((*i)->m_position);
                        m_round_changes_map[(*i)->m_position]->setFighter(-1);

                        if((*i)->m_change_position)
                        {
                            m_round_changes_map[(*i)->m_next_position]->setFighter(-1);
                            m_round_changes_map[(*i)->m_next_position]->setInitFight(false);
                        }
                    }
                    else
                    {
                        if(m_planets[new_position]->getFighter() == 0)
                        {
                            checkForChange(new_position);
                            m_round_changes_map[new_position]->setFighter(1);

                            checkForChange((*i)->m_position);
                            m_round_changes_map[(*i)->m_position]->setFighter(-1);

                            if((*i)->m_change_position)
                            {
                                m_round_changes_map[(*i)->m_next_position]->setFighter(-1);
                            }

                        }
                        else
                        {
                            emit changeRouteError();
                        }
                    }

                    (*i)->m_next_position = new_position;
                    (*i)->m_change_position = true;
                    break;
                }
                else
                {
                    emit changeRouteError();
                }

            }
        }
    }
    else {throwError();}
}

void ManageGame::change_transport_route(int planet_id, int transportSpaceCraft_id)
{
    if(m_initialised)
    {
        //checken ob der planet mit der neuen route auch dem player gehört
        if((m_planets[(unsigned long)planet_id])->getOwner() == m_player_id && (planet_id != m_base))
        {
            for (auto i = m_transportSpaceCraftslist.begin(); i != m_transportSpaceCraftslist.end(); i++)
            {
                if ((*i)->m_id == transportSpaceCraft_id){
                    (*i)->m_tmp_route.clear();
                    (*i)->m_current_route.clear();
                    (*i)->m_tmp_route = m_planetmap->getPath((*i)->m_position, planet_id);
                    (*i)->m_current_route = m_planetmap->getPath(planet_id, m_base);

                    if((*i)->m_tmp_route.size() >= 2)
                    {
                        if((*i)->m_first_turn)
                        {
                            (*i)->m_route_iterator = ++(*i)->m_tmp_route.begin();
                            (*i)->m_first_turn = false;
                        }
                        else
                        {
                            (*i)->m_route_iterator = (*i)->m_tmp_route.begin();
                        }
                        (*i)->m_next_position = *((*i)->m_route_iterator);
                        (*i)->m_to_new_route = true;
                        (*i)->m_to_base = false;
                        (*i)->m_to_mine = false;

                    }
                    else
                    {
                        (*i)->m_route_iterator = (*i)->m_current_route.begin();
                        (*i)->m_to_new_route = false;
                        (*i)->m_to_base = true;
                        (*i)->m_to_mine = false;
                    }
                }
            }
        }
        else
        {
            emit changeRouteError();
        }
    }
    else {throwError();}
}

void ManageGame::next_round()
{
    std::cerr << __LINE__ << ", " << __PRETTY_FUNCTION__ << "\n";
    if(m_initialised)
    {
        //Stop Timer
        emit stopTimer();
        //Update all Ore and Stored ore on PlayerChanges
        for (auto it_p = m_planets.begin(); it_p != m_planets.end(); it_p++)
        {
                         (*it_p)->printPlanet();
            if((*it_p)->getOwner() == m_player_id)
            {
                if((*it_p)->getOre() >= setting.value("Resourcen/Abbaurate").toInt() * (*it_p)->getMine())
                {
                    checkForChange((*it_p)->getID());
                    m_round_changes_map[(*it_p)->getID()]->setOre( -1 * (*it_p)->getMine() * setting.value("Resourcen/Abbaurate").toInt());
                    m_round_changes_map[(*it_p)->getID()]->setStoredOre((*it_p)->getMine() * setting.value("Resourcen/Abbaurate").toInt());

                    if((*it_p)->getID() == m_base)
                    {
                        m_current_resource += (*it_p)->getMine() * setting.value("Resourcen/Abbaurate").toInt();
                    }
                }else
                {
                    checkForChange((*it_p)->getID());
                    m_round_changes_map[(*it_p)->getID()]->setOre(-(*it_p)->getOre());
                    m_round_changes_map[(*it_p)->getID()]->setStoredOre((*it_p)->getOre());

                    if((*it_p)->getID() == m_base)
                    {
                        m_current_resource += (*it_p)->getOre();
                    }
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


        //Check if one player now owns the both base's
        if(m_player_id == m_planets.at(0)->getOwner()
                && m_planets.at((unsigned long) m_planetmap->getNumberOfPlanets()-1)->getOwner() == m_player_id)
        {
            emit end_game(true);
        }
        if(m_player_id != m_planets.at(0)->getOwner()
                && m_planets.at((unsigned long) m_planetmap->getNumberOfPlanets() -1)->getOwner() != m_player_id)
        {
            emit end_game(false);
        }

        //TODO Next 2d-round
        updateStats();

        emit updateScene();
        std::cerr << "\t " << __LINE__ << __FUNCTION__ << " updateScene called " << __LINE__ << std::endl;
        emit updateInfobar();
        emit resetTimer();
    }

    emit updateInfobar();
    // @ahaker send initpacket
    std::list<PlanetChanges> changes;
    std::cerr << "\t" << __LINE__ << ", size of m_round_changes_list: " << m_round_changes_list.size() << "\n" ;
    for(auto x : m_round_changes_list)
    {
        //std::cerr << "\t" << __LINE__ << __FUNCTION__ << ", size of m_round_changes_list: " << m_round_changes_list.size() << "\n" ;
        changes.push_back(*x);
    }
    //PlanetChanges planetchanges(PlanetChanges::Owner::UNASSIGN ,3,1,1,1,1,1,0,1);
    //changes.push_back(planetchanges);
    //PlanetChanges planetchangess(PlanetChanges::Owner::UNASSIGN ,1,1,1,1,1,1,0,1);
    //changes.push_back(planetchangess);

    client_global.m_planet_changes_received = false;
    std::cerr << "\t" << __LINE__ << __FUNCTION__ << ", " << ", changes.size()" << changes.size() << "\n";
    client_global.SendPlanetChanges(changes.size(), changes);
    if(!changes.empty())
    {
        changes.erase(changes.begin());
    }
    client_global.readData();
    if(client_global.init_received){
        std::cerr << "\t" << __LINE__ << __FUNCTION__<< "----------------------------------------\n";
        emit goto3DScene();
    }else if (client_global.m_planet_changes_received) {
        std::cerr << "\t" << __LINE__ << __FUNCTION__<< "----------------------------------------\n";
        //apply changes
    }
    //client_global.init_received = false;
    while(!client_global.init_received || !client_global.m_planet_changes_received) {
        client_global.rerequest_planet_changes();
        client_global.wait_for_readData(100);
        if(client_global.init_received){
            client_global.wait_for_readData(500);
            emit goto3DScene();
            break;
        }else if (client_global.m_planet_changes_received) {
        //apply changes
            break;
        }
        sleep(1);
        std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
    }
    //client_global.send_reset_planet_changes(); @ahaker beim merge wegefallen
    //client_global.m_planet_changes_received = false; @ahaker beim merge wegefallen
    client_global.wait_for_readData(100);
    std::cerr << "\t" << __LINE__ << __FUNCTION__<< "\n";
    std::cerr << "\t" << __LINE__ << __FUNCTION__<< " p_changes_current.size(): " << client_global.p_changes_current.size() << "\n";
    // @ahaker apply all changes
    for (auto it_c = client_global.p_changes_current.begin(); it_c != client_global.p_changes_current.end(); it_c++)
    {
        //std::cerr << "\t" << __LINE__ << __FUNCTION__<< " apply planet changes\n";
        //Applay changes
        m_planets.at((unsigned long)it_c->getID())->updatePlanet(std::make_shared<PlanetChanges>(*(it_c)));

    }

    for (auto i = m_attackSpaceCraftslist.begin(); i != m_attackSpaceCraftslist.end(); ) {
        if( (m_planets.at((*i)->m_position)->getOwner() == (*i)->m_owner) || (PlanetChanges::UNASSIGN == m_planets.at((*i)->m_position)->getOwner()) ){
            i++;
        }else {
            m_attackSpaceCraft_number -= 1;
            i = m_attackSpaceCraftslist.erase(i);
        }
        //if ((*i)->m_id == fighter_id){ break; }
    }

        updateStats();
        emit updateScene();
        std::cerr << "\t " << __LINE__ << __FUNCTION__ << " updateScene called " << __LINE__ << std::endl;
        emit updateInfobar();
}

void ManageGame::planet_apply_updates()
{
    for (auto it_c = client_global.p_changes_current.begin(); it_c != client_global.p_changes_current.end(); it_c++)
    {
        std::cerr << "\t" << __LINE__ << __FUNCTION__<< " apply planet changes\n";
        //Applay changes
        m_planets.at((unsigned long)it_c->getID())->updatePlanet(std::make_shared<PlanetChanges>(*(it_c)));
    }

    for (auto i = m_attackSpaceCraftslist.begin(); i != m_attackSpaceCraftslist.end(); ) {
        if( (m_planets.at((*i)->m_position)->getOwner() == (*i)->m_owner) || (PlanetChanges::UNASSIGN == m_planets.at((*i)->m_position)->getOwner()) ){
            i++;
        }else {
            m_attackSpaceCraft_number -= 1;
            i = m_attackSpaceCraftslist.erase(i);
        }
        //if ((*i)->m_id == fighter_id){ break; }
    }
    updateStats();
    emit updateScene();
    std::cerr << "\t " << __LINE__ << __FUNCTION__ << " updateScene called " << __LINE__ << std::endl;
    emit updateInfobar();
}

void ManageGame::end_game(bool winning)
{
    if(winning)
    {
        emit goToWin();
    }
    else
    {
        emit gotoLoose();
    }
    
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
            if(planet->getOre() > 0)
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
    m_resource_per_time = m_global_mines * setting.value("Resourcen/Abbaurate").toInt();

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
    for(auto i = m_transportSpaceCraftslist.begin(); i != m_transportSpaceCraftslist.end(); i ++)
    {
        if((*i)->m_to_new_route)
        {
            if((*i)->m_route_iterator == (*i)->m_tmp_route.end())
            {
                (*i)->m_route_iterator = (*i)->m_current_route.begin();
                (*i)->m_to_base = true;
                (*i)->m_to_mine = false;
                (*i)->m_to_new_route = false;
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);

                //transporter lädt erz
                (*i)->m_ore = transporter_stored_ore((*i)->m_position);

            }
            else
            {
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);
            }
        }

        if((*i)->m_to_base)
        {
            if((*i)->m_route_iterator == (*i)->m_current_route.end())
            {
                (*i)->m_to_base = false;
                (*i)->m_to_mine = true;
                (*i)->m_to_new_route = false;

                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_route_iterator--;
                (*i)->m_next_position = *((*i)->m_route_iterator--);

                m_current_resource += (*i)->m_ore;
                updateBase((*i)->m_ore);
                (*i)->m_ore = 0;

                emit updateInfobar();

            }
            else
            {
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);
            }
        }

        if((*i)->m_to_mine)
        {
            if((*i)->m_route_iterator == (*i)->m_current_route.begin())
            {
                (*i)->m_to_base = true;
                (*i)->m_to_mine = false;
                (*i)->m_to_new_route= false;

                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator++);

                //transporter lädt erz
                (*i)->m_ore = transporter_stored_ore((*i)->m_next_position);

            }
            else
            {
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_next_position = *((*i)->m_route_iterator--);
            }
        }

        if(m_planets[*(*i)->m_current_route.begin()]->getOwner() != m_player_id)
        {
            (*i)->m_tmp_route = m_planetmap->getPath((*i)->m_position, m_base);
            (*i)->m_current_route.clear();
        }
    }
}

int ManageGame::transporter_stored_ore(int transporter_position)
{
    int tmp = 0;
    //Check for alrady existing change for this planet
    auto search = m_round_changes_map.find(transporter_position);
    if (search != m_round_changes_map.end())
    {
        if(m_planets[transporter_position]->getStoredOre() >= setting.value("Resourcen/Transporterkapazitaet").toInt())
        {
            search->second->setStoredOre(-setting.value("Resourcen/Transporterkapazitaet").toInt());
            tmp = setting.value("Resourcen/Transporterkapazitaet").toInt();

        }else if(m_planets[transporter_position]->getStoredOre() != 0)
        {
            search->second->setStoredOre(-(m_planets[transporter_position]->getStoredOre()));
            tmp = m_planets[transporter_position]->getStoredOre();
        }
    }
    else
    {
        PlanetChanges::Ptr change = std::make_shared<PlanetChanges>(PlanetChanges(transporter_position));

        if(m_planets[transporter_position]->getStoredOre() >= setting.value("Resourcen/Transporterkapazitaet").toInt())
        {
            change->setStoredOre(-setting.value("Resourcen/Transporterkapazitaet").toInt());
            tmp = setting.value("Resourcen/Transporterkapazitaet").toInt();


        }else if(m_planets[transporter_position]->getStoredOre() != 0)
        {
            change->setStoredOre(-(m_planets[transporter_position]->getStoredOre()));
            tmp = m_planets[transporter_position]->getStoredOre();
        }

        m_round_changes_map[transporter_position] = change;
    }
    return tmp;
}


void ManageGame::checkForChange(int planet_id)
{
    auto search = m_round_changes_map.find(planet_id);
    if (search == m_round_changes_map.end())
    {
        m_round_changes_map[planet_id] = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
    }
}

void ManageGame::updateBase(int ore)
{
    auto search = m_round_changes_map.find(m_base);
    if (search == m_round_changes_map.end())
    {
        //Add in not found
        m_round_changes_map[m_base] = std::make_shared<PlanetChanges>(PlanetChanges(m_base));
    }
    m_round_changes_map[m_base]->setStoredOre(ore);
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
        std::cerr << "\t" << __LINE__ << __FUNCTION__ << ", m_player_id: " << m_player_id << ", m_base: " << m_base << "\n";

        //set Satrt planet Owner
        //This is HQ
        if(player_id == PlanetChanges::PLAYER1)
        {
            PlanetChanges::Ptr change1 = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
            change1->setOwner(PlanetChanges::PLAYER1);
            change1->setStoredOre(m_current_resource);
            PlanetChanges::Ptr change2 = std::make_shared<PlanetChanges>(PlanetChanges(m_planetmap->getNumberOfPlanets() -1));
            change2->setOwner(PlanetChanges::PLAYER2);
            m_planets.at((unsigned long)planet_id)->updatePlanet(change1);
            m_planets.at((unsigned long)m_planetmap->getNumberOfPlanets() -1)->updatePlanet(change2);
            std::cerr << "-------------" << m_player_id << "-----------------\n";
            std::cerr << "-------------" << m_base << "-----------------\n";
            std::cerr << "-------------" << planet_id << "-----------------\n";
        }
        if(player_id == PlanetChanges::PLAYER2)
        {
            PlanetChanges::Ptr change1 = std::make_shared<PlanetChanges>(PlanetChanges(planet_id));
            change1->setOwner(PlanetChanges::PLAYER1);
            PlanetChanges::Ptr change2 = std::make_shared<PlanetChanges>(PlanetChanges(m_planetmap->getNumberOfPlanets() -1));
            change2->setOwner(PlanetChanges::PLAYER2);
            change2->setStoredOre(m_current_resource);
            m_planets.at((unsigned long)planet_id)->updatePlanet(change1);
            m_planets.at((unsigned long)m_planetmap->getNumberOfPlanets() -1)->updatePlanet(change2);
            std::cerr << "-------------" << m_player_id << "-----------------\n";
            std::cerr << "-------------" << m_base << "-----------------\n";
            std::cerr << "-------------" << planet_id << "-----------------\n";
        }

    std::cerr << "\t" << __LINE__ << __FUNCTION__ << "\n";
        //Die am Start verfügbaren Schiffe werden den Listen hinzugefügt
        for (int i = 0; i < m_attackSpaceCraft_number;i++)
        {
            Fighter attackSpaceCraft = std::make_shared<attackspacecraft>(attackspacecraft(i, m_base, m_player_id));
            m_attackSpaceCraftslist.push_back(attackSpaceCraft);
        }
        for (int i = 0; i < m_transportSpaceCraft_number;i++)
        {
            Transporter transportSpaceCraft = std::make_shared<transportspacecraft>(transportspacecraft(i, m_base, m_player_id));
            m_transportSpaceCraftslist.push_back(transportSpaceCraft);
        }
        m_initialised = true;
        emit resetTimer();
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
