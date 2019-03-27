/**
 * @file ManageGame.hpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief 
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
    attackspacecraft(int id, int position) : m_id(id), m_position(position), m_next_position(position), m_change_position(false) {}
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
    transportspacecraft(int id, int position) : m_id(id), m_position(position), m_next_position(position),
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
     * @brief updateSpaceCrafts Alle positionen werden aktualisiert
     */
    void updateSpaceCrafts();

    /**
     * @brief List of all PlanetChanges. This should be send to server
     * @return List of Planetchanges
     */
    std::list<PlanetChanges::Ptr>& get_PlanetCangeList();

    //Getter
    inline int get_current_resource(){return m_current_resource;}

    inline int get_resource_per_time(){return m_resource_per_time;}

    inline int get_transportCpaceCraft_number(){return m_transportSpaceCraft_number;}

    inline int get_attackSpaceCraft_number(){return m_attackSpaceCraft_number;}

    inline std::list<Fighter>& get_attackSpaceCraftList(){return m_attackSpaceCraftslist;}

    inline std::list<Transporter>& get_transportSpaceCraftList(){return m_transportSpaceCraftslist;}

    int transporter_stored_ore(int transporter_position);

    void updateBase(int ore);

  signals:
    void gameover();
    void no_resources(int);
    void updateInfobar();
    void not_ur_planet();
    void not_ur_ship();
    void already_exist();
    void goToScene2D();
    void goto3DScene();
    void changeRouteError();
    void updateScene();
    void noFactory();
    void stopTimer();
    void resetTimer();
    void fighter_already_exist();

  public slots:
    void change_Fighter_position(int new_position, int attackSpaceCraft_id);
    void change_transport_route(int planet_id, int transportSpaceCraft_id);
    void build_factory(int planet_id);
    void build_mine(int planet_id);
    void build_fighter(int planet_id);
    void build_transporter(int planet_id);
    void destroy_fighter(int fighter_id);
    void next_round();
    void end_game();
    void planet_apply_updates();



  private:

    /**
     * @brief Construct a new Manage Game object
     * 
     * @param parent 
     */
    explicit ManageGame(QObject *parent = nullptr);

    /**
     * @brief Interates through all planets and counts mine, fighter and transporter
     *
     */
    void updateStats();

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
