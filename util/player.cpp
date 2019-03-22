#include "player.h"


#define START_RESOURCE 1000
#define START_RESOURCE_PER_TIME 100
#define RESOURCE_PER_MINE 100
#define COST_PER_ATTACKSPACECRAFT 1000
#define COST_PER_TRANSPORTSPACECRAFT 1000
#define COST_PER_MINE 1000
#define COST_PER_SHIPYARD 100
#define START_ATTACKSPACECRAFT_NUMBER 1
#define START_TRANSPORTSPACECRAFT_NUMBER 1

namespace asteroids {

player::player(QObject *parent, int base) : QObject(parent)
{
   //start resourcen festlegen
   m_current_resource = START_RESOURCE;
   m_resource_per_time = START_RESOURCE_PER_TIME;
   m_attackSpaceCraft_number = START_ATTACKSPACECRAFT_NUMBER;
   m_transportSpaceCraft_number = START_TRANSPORTSPACECRAFT_NUMBER;
   m_attackspacecraft_id = m_attackSpaceCraft_number;

   //Die am Start verfügbaren Schiffe werden den Listen hinzugefügt
   for (int i = 1; i <= m_attackSpaceCraft_number;i++) {
       std::shared_ptr<attackspacecraft> attackSpaceCraft = std::make_shared<attackspacecraft>(i, base);
       m_attackSpaceCraftslist.push_back(attackSpaceCraft);
   }
   for (int i = 1; i <= m_transportSpaceCraft_number;i++) {
       std::shared_ptr<transportspacecraft> transportSpaceCraft = std::make_shared<transportspacecraft>(i,base);
       m_transportSpaceCraftslist.push_back(transportSpaceCraft);
   }

   MapFactory& b = MapFactory::getinstance();
   Map::Ptr map = b.getMap("models/01.map");
   m_planets = map->getPlanets();

}

//instace
player& player::getinstance(){
    static player instance;
    return instance;
}

//aufruf wenn planet und darauf enthaltene mienen verloren gehen
void player::lost_planet(int planet_id){
    int tmp_mine_number = m_planets[planet_id]->getMine();
    for (int i = 0; i < planet_id;i++) {
        m_resource_per_time -= RESOURCE_PER_MINE * tmp_mine_number;
    }
}

// werft bauen
void player::build_shipyard(int planet_number){
    if(m_current_resource >= COST_PER_SHIPYARD){
        m_current_resource -= COST_PER_SHIPYARD;

        //änderungen in der liste speichern
        bool exist = false;
        for (auto i = m_round_changes.begin(); i != m_round_changes.end();i++) {
            if((*i)->getID() == planet_number){
                (*i)->setFactorys(1);
                exist = true;
            }
        }
        if(exist == false){
            PlanetChanges::Ptr new_change = std::make_shared<PlanetChanges>(planet_number);
            new_change->setFactorys(1);
            m_round_changes.push_back(new_change);
        }
        //update signal für die info bar
        emit update();
    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }
}

//miene bauen
void player::build_mine(int planet_number){
    if(m_current_resource >= COST_PER_MINE){
        m_current_resource -= COST_PER_MINE;
        m_resource_per_time += RESOURCE_PER_MINE;

        //änderungen in liste speichern
        bool exist = false;
        for (auto i = m_round_changes.begin(); i != m_round_changes.end();i++) {
            if((*i)->getID() == planet_number){
                (*i)->setMines(1);
                exist = true;
            }
        }
        if(exist == false){
            PlanetChanges::Ptr new_change = std::make_shared<PlanetChanges>(planet_number);
            new_change->setMines(1);
            m_round_changes.push_back(new_change);
        }

        emit update();
    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }
}

//aufruf wenn ein attackspacecraft gebaut wird
void player::new_attackSpaceCraft(int planet_number){
    if(m_current_resource >= COST_PER_ATTACKSPACECRAFT){
    m_current_resource -= COST_PER_ATTACKSPACECRAFT;
    m_attackspacecraft_id += 1;
    std::shared_ptr<attackspacecraft> attackSpaceCraft = std::make_shared<attackspacecraft>(m_attackspacecraft_id, planet_number);
    m_attackSpaceCraftslist.push_back(attackSpaceCraft);

    bool exist = false;
    for (auto i = m_round_changes.begin(); i != m_round_changes.end();i++) {
        if((*i)->getID() == planet_number){
            (*i)->setFighter(1);
            exist = true;
        }
    }
    if(exist == false){
        PlanetChanges::Ptr new_change = std::make_shared<PlanetChanges>(planet_number);
        new_change->setFighter(1);
        m_round_changes.push_back(new_change);
    }

    emit update();
    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }

}

// aufruf wenn ein Kampf verloren wurde
void player::destroy_attackSpaceCraft(int number_id){
    for (auto i = m_attackSpaceCraftslist.begin(); i != m_attackSpaceCraftslist.end(); i++) {
        if ((*i)->m_id == number_id){
            i = m_attackSpaceCraftslist.erase(i);
            m_attackSpaceCraft_number -= 1;
        }
    }
    emit update();
}

//aufruf wenn ein transportspacecraft gebaut wird
void player::new_transportSpaceCraft(int planet_number){
    if(m_current_resource >= COST_PER_TRANSPORTSPACECRAFT){
        m_current_resource -= COST_PER_TRANSPORTSPACECRAFT;
        m_transportSpaceCraft_number += 1;
        std::shared_ptr<transportspacecraft> transportSpaceCraft = std::make_shared<transportspacecraft>(m_transportSpaceCraft_number, planet_number);
        m_transportSpaceCraftslist.push_back(transportSpaceCraft);

        emit update();
    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }
}

//aufruf wenn ein attackspacecraft zu einem anderen planeten fliegen soll
void player::change_attackSpaceCraft_position(int new_position, int attackSpaceCraft_id){
    for (auto i = m_attackSpaceCraftslist.begin(); i != m_attackSpaceCraftslist.end(); i++) {
        if ((*i)->m_id == attackSpaceCraft_id){
            (*i)->m_next_position = new_position;
            (*i)->m_change_position = true;

        }
    }
}

//aufruf wenn einem transportspacecraft einem neuen planeten zugeordnet wird
void player::change_transportSpaceCraft_route(std::list<int> new_route, std::list<int> tmp_route, int transportSpaceCraft_id){
    //checken ob der planet mit der neuen route auch dem player gehört
    if((m_planets[(*new_route.begin())])->getOwner() == m_player_id){
        for (auto i = m_transportSpaceCraftslist.begin(); i != m_transportSpaceCraftslist.end(); i++) {
            if ((*i)->m_id == transportSpaceCraft_id){
                (*i)->m_tmp_route = tmp_route;
                (*i)->m_current_route = new_route;
                (*i)->m_route_iterator = (*i)->m_tmp_route.begin();
                (*i)->m_to_new_route = true;
            }
        }
    }else {
        //falscher owner
    }
}

void player::quit(){
    emit gameover();
}

//aufruf wenn eine neue runde gestartet wird
void player::new_round(){
    m_current_resource += m_resource_per_time;
    m_round_changes.clear();

    for (auto i = m_attackSpaceCraftslist.begin();i != m_attackSpaceCraftslist.end(); i++) {
        if((*i)->m_change_position == true){
                (*i)->m_position = (*i)->m_next_position;
                (*i)->m_change_position = false;
        }
    }

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
    if(m_attackSpaceCraft_number == 0){
        emit gameover();
    }
    emit update();
}
}
