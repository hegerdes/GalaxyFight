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

#define START_RESOURCE 1000
#define START_RESOURCE_PER_TIME 100
#define RESOURCE_PER_MINE 100
#define COST_PER_ATTACKSPACECRAFT 1000
#define COST_PER_TRANSPORTSPACECRAFT 1000
#define COST_PER_MINE 1000
#define COST_PER_SHIPYARD 100
#define START_ATTACKSPACECRAFT_NUMBER 1
#define START_TRANSPORTSPACECRAFT_NUMBER 1

#include <QObject> 
#include <list>
#include <map>

#include "../rendering/2D/MapFactory.hpp"
#include "../rendering/2D/PlanetChanges.hpp"
#include "../rendering/2D/Map.hpp"

namespace asteroids
{
class ManageGame : public QObject 
{
    Q_OBJECT

  public:

    inline int get_current_resource(){return m_current_resource;}

    inline int get_resource_per_time(){return m_resource_per_time;}

    inline int get_transportCpaceCraft_number(){return m_transportSpaceCraft_number;}

    inline int get_attackSpaceCraft_number(){return m_attackSpaceCraft_number;}

  signals:
    void gameover();
    void no_resources();
    void update();

  public slots:
    void build_factory(int planet_id);
    void build_mine(int planet_id);
    void build_fighter(int planet_id);
    void build_transporter(int planet_id);
    void next_round();
    void end_game();

    /**
     * @brief Singelton get the instance
     * 
     * @return ManageGame& Reference to ManageGame
     */
    static ManageGame& getinstance();

    /**
     * @brief Interates through all planets and counts mine, fighter and transporter
     * 
     */
    void updateStats();



  private:

    /**
     * @brief Construct a new Manage Game object
     * 
     * @param parent 
     * @param base 
     */
    explicit ManageGame(QObject *parent = nullptr, int base = 0);

    /**
     * @brief Destroy the Manage Game object
     * 
     */
    virtual ~ManageGame();



    //The instance
    static ManageGame* instance;

    //Pointer to the map
    Map::Ptr m_planetmap;

    //Planets
    Map::VecPtr m_planets;

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
    Planet::Owner m_player_id;

    //counter für die Kampfschiff-id
    int m_attackspacecraft_id;

    //save changes in list
    std::list<PlanetChanges::Ptr> m_round_changes_list;

    //save changes in map
    std::map<int,PlanetChanges::Ptr> m_round_changes_map;


};

}//asteroids



#endif