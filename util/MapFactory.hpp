/**
 * @file MapFactory.hpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef MAPFACRORY_HPP
#define MAPFACTORY_HPP

#pragma once
#include <string>

#include "../view/items/Map.hpp"

namespace asteroids
{
class MapFactory
{
  public:
    
    /**
     * @brief Get the one instance of MapFactors
     * 
     * @return MapFactory& Reference to the MapFactory
     */
    static MapFactory& getinstance();


    /**
     * @brief Get the Map object
     * @param path The path to mapfile
     * 
     * @return Map::Ptr A smartpointer to a Map
     */
    Map::Ptr getMap(std::string path);

  private:
    
    /**
     * @brief Construct a new Map Factory object
     * 
     */
    MapFactory();

    /**
     * @brief Destroy the Map Factory object
     * 
     */
    virtual ~MapFactory();

    //store the map(s)
    std::map<std::string,Map::Ptr> m_maps;

    //the instance
    static MapFactory* instance;
};

}//asteroids


#endif