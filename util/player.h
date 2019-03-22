#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <list>
#include "rendering/2D/PlanetChanges.hpp"
#include <map>
#include "rendering/2D/Planet.hpp"
#include "rendering/2D/Map.hpp"
#include "rendering/2D/Planet.hpp"
#include "rendering/2D/MapFactory.hpp"

namespace asteroids {

struct attackspacecraft{
    //constructor
    attackspacecraft(int id, int position) : m_id(id), m_position(position), m_change_position(false) {}
    //id für einzelne Schiffe
    int m_id;
    //aktuelle position des Schiffes
    int m_position;
    //nächste position, wenn aktuelle geändert werden soll
    int m_next_position;
    //flag ob es ein positionswechsel in der nächsten runde geben soll
    bool m_change_position;
};

struct transportspacecraft{
    //constructor
    transportspacecraft(int id, int position) : m_id(id), m_position(position), m_to_new_route(false) {}
    //id für einzelne Schiffe
    int m_id;
    //aktuelle position
    int m_position;
    //nächste position der route
    int m_next_position;
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

class player : public QObject
{
    Q_OBJECT
public:

    inline int get_current_resource(){return m_current_resource;}

    inline int get_resource_per_time(){return m_resource_per_time;}

    inline int get_transportCpaceCraft_number(){return m_transportSpaceCraft_number;}

    inline int get_attackSpaceCraft_number(){return m_attackSpaceCraft_number;}

    inline std::list<std::shared_ptr<attackspacecraft>> get_attackSpaceCraftList(){return m_attackSpaceCraftslist;}

    inline std::list<std::shared_ptr<transportspacecraft>> get_transportSpaceCraftList(){return m_transportSpaceCraftslist;}

    inline void set_playerid(int id){m_player_id = id;}

    static player& getinstance();


signals:

    //signal wenn player aufgibt oder er verloren hat
    void gameover();

    //signal wenn nicht genug resourcen vorhanden sind um etwas zu bauen
    void no_resources();

    //signal wenn sich etwas an den attributen geändert hat um die info bar zu updaten
    void update();

public slots:

    //wird aufgerufen wenn ein Kampf um einen planeten verloren wurde
    void lost_planet(int planet_id);

    void build_shipyard(int planet_id);

    void build_mine(int planet_id);

    void new_attackSpaceCraft(int planet_id);

    void destroy_attackSpaceCraft(int attackSpaceCraft_id);

    void new_transportSpaceCraft(int planet_id);

    void change_transportSpaceCraft_route(std::list<int> route, std::list<int> tmp_route, int transportSpaceCraft_id);

    void change_attackSpaceCraft_position(int new_planet_id, int attackSpaceCraft_id);

    void new_round();

    void quit();

private:
    //spielmap
    Map::VecPtr m_planets;

    //private constructor für singelton
    explicit player(QObject *parent = nullptr, int base = 0);

    //liste von Kampfschiffen
    std::list<std::shared_ptr<attackspacecraft>> m_attackSpaceCraftslist;

    //liste von Transportschiffen
    std::list<std::shared_ptr<transportspacecraft>> m_transportSpaceCraftslist;

    //aktuelle anzahl an resourcen
    int m_current_resource;

    //aktuelle anzahl an resourcen pro zeit
    int m_resource_per_time;

    //aktuelle anzahl an Kampfschiffen
    int m_attackSpaceCraft_number;

    //aktuelle anzahl an Transportschiffen
    int m_transportSpaceCraft_number;

    //id zum überprüfen des owners von planeten
    //wird vom server festgelegt
    int m_player_id;

    //counter für die Kampfschiff-id
    int m_attackspacecraft_id;

    //map um die einzelnen änderungen an planeten zu speichern
    std::list<PlanetChanges::Ptr> m_round_changes;



};
}
#endif // PLAYER_H
