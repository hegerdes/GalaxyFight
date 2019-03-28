#ifndef CLIENT_DATA_H
#define CLIENT_DATA_H

#include "../math/Vector.hpp"
#include "flags.h"
#include <vector>

/**
 * @brief The client_data struct represents the data that will be sent over the socket when a
 * 3D fight starts
 * @param position  The position of the fighter
 * @param x,y,z     Axis the axis of the fighter
 * @param shot  stores if the fighter shot a bullet or not
 * @param bullet_id     the id of each bullet that got shot
 * @param living    stores if the player is living or dead after he got shot
 * @param m_first_connect stores if this is the first connect of the client
 */

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

    //2D Teil
    std::string name = "Name undefined";
    bool m_first_connect = false;
};

#endif // CLIENT_DATA_H
