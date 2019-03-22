#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <list>
#include "rendering/2D/PlanetChanges.hpp"
#include <map>

namespace asteroids {

struct attackspacecraft{
    attackspacecraft(int start_id, int start_position) : id(start_id), position(start_position), change_position(false) {}
    int id;
    int position;
    int next_position;
    bool change_position;
};

struct transportspacecraft{
    transportspacecraft(int start_id, int start_position) : id(start_id), position(start_position) {}
    int id;
    int position;
    int next_position;
    bool to_base;
    bool to_mine;
    std::list<int> current_route;
    std::list<int> tmp_route;
    std::list<int>::iterator route_iterator;
};

class player : public QObject
{
    Q_OBJECT
public:

    inline int get_current_resource(){return current_resource;}

    inline int get_resource_per_time(){return resource_per_time;}

    inline int get_transportCpaceCraft_number(){return transportSpaceCraft_number;}

    inline int get_attackSpaceCraft_number(){return attackSpaceCraft_number;}

    inline std::list<attackspacecraft*> get_attackSpaceCraftList(){return attackSpaceCraftslist;}

    inline std::list<transportspacecraft*> get_transportSpaceCraftList(){return transportSpaceCraftslist;}

    inline void set_playerid(int id){player_id = id;}

    player& getinstance();


signals:

    void gameover();

    void no_resources();

    void update();

public slots:

    void decrease_resource_per_time(int mine_number);

    void build_shipyard(int planet_number);

    void build_mine(int planet_number);

    void new_attackSpaceCraft(int start_position);

    void destroy_attackSpaceCraft(int number_id);

    void new_transportSpaceCraft(int start_position);

    void change_transportSpaceCraft_route(std::list<int> route, int transportSpaceCraft_id);

    void change_attackSpaceCraft_position(int new_position, int attackSpaceCraft_id);

    void new_round();

    void quit();

private:
    //private constructor für singelton
    explicit player(QObject *parent = nullptr, int base = 0);

    //liste von Kampfschiffen
    std::list<attackspacecraft*> attackSpaceCraftslist;

    //liste von Transportschiffen
    std::list<transportspacecraft*> transportSpaceCraftslist;

    //aktuelle anzahl an resourcen
    int current_resource;

    //aktuelle anzahl an resourcen pro zeit
    int resource_per_time;

    //aktuelle anzahl an Kampfschiffen
    int attackSpaceCraft_number;

    //aktuelle anzahl an Transportschiffen
    int transportSpaceCraft_number;

    //id zum überprüfen des owners von planeten
    //wird vom server festgelegt
    int player_id;

    //counter für die Kampfschiff-id
    int attackspacecraft_id;

    //map um die einzelnen änderungen an planeten zu speichern
    std::list<PlanetChanges> round_changes;

};
}
#endif // PLAYER_H
