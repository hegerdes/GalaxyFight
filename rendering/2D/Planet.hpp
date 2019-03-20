/**
 * @file Planet.hpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef PLANET_HPP
#define PLANET_HPP


#include <stdio.h>
#include <string>
#include <memory>

#include "../../math/Vector.hpp"

namespace asteroids
{

class Planet
{
  public:
    using Ptr = std::shared_ptr<Planet>;
    enum Owner{UNASSIGN, PLAYER1, PLAYER2};

    Planet(std::string name, Vector3f pos);

    float dist(Vector3f other);

  private:

    std::string m_planet_name;

    unsigned int m_num_of_ore;

    Vector3f m_planet_pos;

    int m_num_factory;

    int m_num_mine;

    int m_num_fighters;

    int m_num_transporter;

    Owner m_owned;
    
    

};

}//asteroids

#endif