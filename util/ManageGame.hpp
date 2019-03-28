/**
 * @file ManageGame.hpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief This is the 2D GameLogic and a singelton. It handals Signals from InfoBar 
 *        and Updated the Infobar. I checks if player is allowed to build new 
 *        stuff. All player changede (Ore, Fighter ...) are saved and will be send 
 *        to server if round ends.
 *        ManageGame will send Singals if player wants to do somthing unallowed 
 *        and if the scene should be changed
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef MANAGEGANE_HPP
#define MANAGEGAME_HPP

#pragma once

#include <QObject> 
#include <iostream>
#include <list>
#include <stdexcept>
#include <map>

#include "../rendering/2D/MapFactory.hpp"
#include "../rendering/2D/Map.hpp"
#include "init_file.h"

namespace asteroids
{

struct attackspacecraft{
    //constructor
    attackspacecraft(int id, int position, PlanetChanges::Owner owner) : m_id(id), m_position(position),
        m_next_position(position), m_change_position(true), m_owner(owner) {}
    //id für einzelne Schiffe
    int m_id;
    //owner
    PlanetChanges::Owner m_owner;
    //aktuelle position des Schiffes
    int m_position;
    //nächste position, wenn aktuelle geändert werden soll
    int m_next_position;
    //flag ob es ein positionswechsel in der nächsten runde geben soll
    bool m_change_position;
};

struct transportspacecraft{
    //constructor
    transportspacecraft(int id, int position, PlanetChanges::Owner owner) : m_id(id), m_owner(owner), m_position(position), m_next_position(position),
        m_to_new_route(false), m_to_base(false), m_to_mine(false), m_current_route(), m_tmp_route(), m_route_iterator() {}
    //id für einzelne Schiffe
    int m_id;
    //owner
    PlanetChanges::Owner m_owner;
    //aktuelle position
    int m_position;
    //nächste position der route
    int m_next_position;
    //geladenes erz
    int m_ore;
    //flag ob Schiff noch zur route fliegen muss
    bool m_to_new_route;
    //flag ob Schiff zur base fliegt
    bool m_to_base;
    //flag ob Schiff zur miene fliegt
    bool m_to_mine;
    //liste der planeten für die aktuelle dauerhafte route
    std::list<int> m_current_route;
    //liste der planeten, um zum planeten des routenbegins zu kommen
    std::list<int> m_tmp_route;
    //iterator um durh die liste zu gehen
    std::list<int>::iterator m_route_iterator;
};


class ManageGame : public QObject 
{
    Q_OBJECT

  public:
    using Fighter = std::shared_ptr<attackspacecraft>;
    using Transporter = std::shared_ptr<transportspacecraft>;

    /**
     * @brief initialize_player initializiert die die ManageGame Instanz
     * @param player_id ist die eigene id
     * @param planet_id ist die id vom Hauptquatier
     * @return
     */
    void initialize_player(PlanetChanges::Owner player_id, int planet_id);

    /**
     * @brief Singelton get the instance
     * 
     * @return ManageGame& Reference to ManageGame
     */
    static ManageGame* getinstance();

    /**
     * @brief List of all PlanetChanges. This should be send to server
     * @return List of Planetchanges
     */
    std::list<PlanetChanges::Ptr>& get_PlanetCangeList();

    /**
     * @brief Get the current resource number
     * 
     * @return int Number of ressources the player has
     */
    inline int get_current_resource(){return m_current_resource;}

    /**
     * @brief Get the resource per time number
     * 
     * @return int Number of ressources plyer get every round
     */
    inline int get_resource_per_time(){return m_resource_per_time;}

    /**
     * @brief Get the transportCpaceCraft number number
     * 
     * @return int Number of transporters
     */
    inline int get_transportCpaceCraft_number(){return m_transportSpaceCraft_number;}

    /**
     * @brief Get the attackSpaceCraft number
     * 
     * @return int Number of fighter
     */
    inline int get_attackSpaceCraft_number(){return m_attackSpaceCraft_number;}

    /**
     * @brief Get the attackSpaceCraftList 
     * 
     * @return std::list<Fighter>& List with all fighters
     */
    inline std::list<Fighter>& get_attackSpaceCraftList(){return m_attackSpaceCraftslist;}

    /**
     * @brief Get the transportSpaceCraftList 
     * 
     * @return std::list<Transporter>& List with all transporters
     */
    inline std::list<Transporter>& get_transportSpaceCraftList(){return m_transportSpaceCraftslist;}

  signals:
    void gameover();
    void no_resources(int);
    void updateInfobar();
    void not_ur_planet();
    void not_ur_ship();
    void already_exist();
    void goToScene2D();
    void goto3DScene();
    void goToWin();
    void gotoLoose();
    void changeRouteError();
    void updateScene();
    void noFactory();
    void stopTimer();
    void resetTimer();
    void fighter_already_exist();

  public slots:
    
    /**
     * @brief Updated the FighterPos
     * 
     * @param new_position New pos
     * @param attackSpaceCraft_id  ID of the fighter
     */
    void change_Fighter_position(int new_position, int attackSpaceCraft_id);
    
    /**
     * @brief Calcs the new route for the transporter
     * 
     * @param planet_id The planet to travel to
     * @param transportSpaceCraft_id Id of the Transporter
     */
    void change_transport_route(int planet_id, int transportSpaceCraft_id);
    
    /**
     * @brief Player builds a new factors
     * 
     * @param planet_id ID of the planet to build on
     */
    void build_factory(int planet_id);
    
    /**
     * @brief Player builds new mine
     * 
     * @param planet_id ID of the planet to build on
     */
    void build_mine(int planet_id);
    
    /**
     * @brief Player builds new fighter
     * 
     * @param planet_id ID of the planet to build on
     */
    void build_fighter(int planet_id);
    
    /**
     * @brief Player builds new transporter
     * 
     * @param planet_id ID of the planet to build on
     */
    void build_transporter(int planet_id);
    
    /**
     * @brief Fight destorys fighter
     * 
     * @param fighter_id ID of the fighter to destry
     */
    void destroy_fighter(int fighter_id);
    
    /**
     * @brief If timer endex or player clickt weiter
     *        Changes will be sent to server.
     *        Wait for respose. If fight init from server
     *        switch scene. Else get updated and aply
     * 
     */
    void next_round();
    
    /**
     * @brief If one player has the base of the other or one player quits
     * 
     */
    void end_game(bool);



  private:

    /**
     * @brief Construct a new Manage Game
     * 
     * @param parent Windowparent
     */
    explicit ManageGame(QObject *parent = nullptr);

    /**
     * @brief Interates through all planets and counts mine, fighter and transporter and updated the infobar
     *
     */
    void updateStats();

    /**
     * @brief Checks if a change alrady exists. If not it will be created
     * 
     * @param planet_id ID of Planet
     */
    void checkForChange(int planet_id);

    /**
     * @brief Update the ore in base
     * 
     * @param ore Ore to add
     */
    void updateBase(int ore);

    /**
     * @brief Updats one ore on the transprter
     * 
     * @param transporter_position The Pos of transporter
     * @return int 
     */
    int transporter_stored_ore(int transporter_position);

    /**
     * @brief updateSpaceCrafts Alle positionen werden aktualisiert
     */
    void updateSpaceCrafts();

    /**
     * @brief Throws a error if not inizlized
     */
    [[ noreturn ]] void throwError();

    /**
     * @brief Destroy the Manage Game object
     *
     */
    virtual ~ManageGame();



    //The instance
    static ManageGame* instance;

    //If alrady initialise
    bool m_initialised;

    //Pointer to the map
    Map::Ptr m_planetmap;

    //Planets
    Map::VecPtr m_planets;

    //planeten id vom Hauptquatier
    int m_base;

    //Current ressources
    int m_current_resource;

    //Current ressources per time
    int m_resource_per_time;

    //Global number of fighters
    int m_attackSpaceCraft_number;

    //global number of transporters
    int m_transportSpaceCraft_number;

    //global number of mines
    int m_global_mines;

    //id for ownerckeck
    PlanetChanges::Owner m_player_id;

    //counter für die Kampfschiff-id
    int m_attackspacecraft_id;

    //save changes in list
    std::list<PlanetChanges::Ptr> m_round_changes_list;

    //save changes in map
    std::map<int,PlanetChanges::Ptr> m_round_changes_map;

    //liste von Kampfschiffen
    std::list<Fighter> m_attackSpaceCraftslist;

    //liste von Transportschiffen
    std::list<Transporter> m_transportSpaceCraftslist;

};

}//asteroids



#endif
