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
   current_resource = START_RESOURCE;
   resource_per_time = START_RESOURCE_PER_TIME;
   attackSpaceCraft_number = START_ATTACKSPACECRAFT_NUMBER;
   transportSpaceCraft_number = START_TRANSPORTSPACECRAFT_NUMBER;
   attackspacecraft_id = attackSpaceCraft_number;

   //Die am Start verfügbaren Schiffe werden den Listen hinzugefügt
   for (int i = 1; i <= attackSpaceCraft_number;i++) {
       attackspacecraft *start_attackSpaceCraft = new attackspacecraft(i,base);
       attackSpaceCraftslist.push_back(start_attackSpaceCraft);
   }
   for (int i = 1; i <= transportSpaceCraft_number;i++) {
       transportspacecraft *start_transportSpaceCraft = new transportspacecraft(i,base);
       transportSpaceCraftslist.push_back(start_transportSpaceCraft);
   }

   //connections

}

//instace
player& player::getinstance(){
    static player instance;
    return instance;
}

//aufruf wenn planet und darauf enthaltene mienen verloren gehen
void player::decrease_resource_per_time(int mine_number){
    for (int i = 0; i < mine_number;i++) {
        resource_per_time -= RESOURCE_PER_MINE;
    }
}

// werft bauen
void player::build_shipyard(int planet_number){
    if(current_resource >= COST_PER_SHIPYARD){
        current_resource -= COST_PER_SHIPYARD;

        PlanetChanges new_change(planet_number);
        new_change.setFactorys(1);
        round_changes.push_back(new_change);
    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }
}

//miene bauen
void player::build_mine(int planet_number){
    if(current_resource >= COST_PER_MINE){
        current_resource -= COST_PER_MINE;
        resource_per_time += RESOURCE_PER_MINE;

        PlanetChanges new_change(planet_number);
        new_change.setMines(1);
        round_changes.push_back(new_change);
    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }
}

//aufruf wenn ein attackspacecraft gebaut wird
void player::new_attackSpaceCraft(int start_position){
    if(current_resource >= COST_PER_ATTACKSPACECRAFT){
    current_resource -= COST_PER_ATTACKSPACECRAFT;
    attackspacecraft_id += 1;
    attackspacecraft *attackSpaceCraft = new attackspacecraft(attackSpaceCraft_number,start_position);
    attackSpaceCraftslist.push_back(attackSpaceCraft);

    PlanetChanges new_change(start_position);
    new_change.setFighter(1);
    round_changes.push_back(new_change);

    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }

}

// aufruf wenn ein Kampf verloren wurde
void player::destroy_attackSpaceCraft(int number_id){
    for (auto i = attackSpaceCraftslist.begin(); i != attackSpaceCraftslist.end(); i++) {
        if ((*i)->id == number_id){
            i = attackSpaceCraftslist.erase(i);
            attackSpaceCraft_number -= 1;
        }
    }
}

//aufruf wenn ein transportspacecraft gebaut wird
void player::new_transportSpaceCraft(int start_position){
    if(current_resource >= COST_PER_TRANSPORTSPACECRAFT){
        current_resource -= COST_PER_TRANSPORTSPACECRAFT;
        transportSpaceCraft_number += 1;
        transportspacecraft* transportSpaceCraft = new transportspacecraft(transportSpaceCraft_number, start_position);
        transportSpaceCraftslist.push_back(transportSpaceCraft);

    }else {
        //nicht genug resourcen vorhanden
        emit no_resources();
    }
}

//aufruf wenn ein attackspacecraft zu einem anderen planeten fliegen soll
void player::change_attackSpaceCraft_position(int new_position, int attackSpaceCraft_id){
    for (auto i = attackSpaceCraftslist.begin(); i != attackSpaceCraftslist.end(); i++) {
        if ((*i)->id == attackSpaceCraft_id){
            (*i)->next_position = new_position;
            (*i)->change_position = true;

        }
    }
}

//aufruf wenn einem transportspacecraft einem neuen planeten zugeordnet wird
void player::change_transportSpaceCraft_route(std::list<int> new_route, int transportSpaceCraft_id){
    //checken ob der planet mit der neuen route auch dem player gehört
    if(true){
        for (auto i = transportSpaceCraftslist.begin(); i != transportSpaceCraftslist.end(); i++) {
            if ((*i)->id == transportSpaceCraft_id){
                (*i)->current_route = new_route;
                (*i)->route_iterator = (*i)->current_route.begin();
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
    current_resource += resource_per_time;
    round_changes.clear();

    for (auto i = attackSpaceCraftslist.begin();i != attackSpaceCraftslist.end(); i++) {
        if((*i)->change_position == true){
                (*i)->position = (*i)->next_position;
                (*i)->change_position = false;
        }
    }

    for(auto i = transportSpaceCraftslist.begin(); i != transportSpaceCraftslist.end(); i ++){
        if((*i)->to_base == true){
            if((*i)->route_iterator == (*i)->current_route.end()){
                (*i)->to_base = false;
                (*i)->to_mine = true;
                (*i)->position = *((*i)->route_iterator--);
                (*i)->next_position = *((*i)->route_iterator);
            }else {
                (*i)->position = *((*i)->route_iterator++);
                (*i)->next_position = *((*i)->route_iterator);
            }
        }
        if((*i)->to_mine == true){
            if((*i)->route_iterator == (*i)->current_route.begin()){
                (*i)->to_base = true;
                (*i)->to_mine = false;
                (*i)->position = *((*i)->route_iterator++);
                (*i)->next_position = *((*i)->route_iterator);

            }else {
                (*i)->position = *((*i)->route_iterator--);
                (*i)->next_position = *((*i)->route_iterator);
            }
        }
    }
    if(attackSpaceCraft_number == 0){
        emit gameover();
    }
}
}
