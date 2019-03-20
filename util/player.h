#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <list>
#include <boost/container/vector.hpp>

struct attackspacecraft{
    attackspacecraft(int start_id, int start_position) : id(start_id), position(start_position), change_position(false) {}
    int id;
    int position;
    int next_position;
    bool change_position;
    std::list<int> routeToNewPosition;
    std::list<int>::iterator route_iterator;
};

struct transportspacecraft{
    transportspacecraft(int start_id, int start_position) : id(start_id), position(start_position) {}
    int id;
    int position;
    int next_position;
    bool to_base;
    bool to_mine;
    std::list<int> current_route;
    std::list<int>::iterator route_iterator;
};

class player : public QObject
{
    Q_OBJECT
public:
    explicit player(QObject *parent = nullptr, int base = 0);

signals:
    void gameover();
    void no_resources();

public slots:
    void increase_resource_per_time();
    void decrease_resource_per_time(int mine_number);
    void build_shipyard();
    void build_mine();
    void new_attackSpaceCraft(int start_position);
    void destroy_attackSpaceCraft(int number_id);
    void new_transportSpaceCraft(int start_position);
    void change_transportSpaceCraft_route(std::list<int> route, int transportSpaceCraft_id);
    void change_attackSpaceCraft_position(std::list<int> new_route, int attackSpaceCraft_id);
    void new_round();

private:
    std::list<attackspacecraft*> attackSpaceCraftslist;
    std::list<transportspacecraft*> transportSpaceCraftslist;
    int current_resource;
    int resource_per_time;
    int attackSpaceCraft_number;
    int transportSpaceCraft_number;

};

#endif // PLAYER_H
