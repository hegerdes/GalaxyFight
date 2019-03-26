#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include "../math/Vector.hpp"
#include "flags.h"
#include <vector>

struct client_data {
    //3D Teil
    asteroids::Vector<float> position;
    asteroids::Vector<float> xAxis;
    asteroids::Vector<float> yAxis;
    asteroids::Vector<float> zAxis;
    Bullet_shot shot;
    int bullet_id;
    Living living;

    //physics stuff
    std::vector<int> deleted_bullets_id;
    std::vector<int> deleted_asteroids_id;
    int m_hp;
    int m_enem_hp;

    //2D Teil
    std::string name = "Name undefined";
};

#endif // CLIENT_DATA_H
