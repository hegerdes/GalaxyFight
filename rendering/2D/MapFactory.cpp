/**
 * @file MapFactory.cpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief 
 * @version 0.1
 * @date 2019-03-21
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "MapFactory.hpp"

namespace asteroids
{
    MapFactory::MapFactory()
    {

    }

    MapFactory::~MapFactory()
    {
        
    }

    MapFactory& MapFactory::getinstance()
    {
        static MapFactory instance;
        return instance;
    }

    Map::Ptr MapFactory::getMap(std::string path)
    {
        //Look for map in maps
        auto search = m_maps.find(path);
        if(search != m_maps.end())
        {
            //Map was alrady loaded
            return search->second;
        }

        Map::Ptr map = std::make_shared<Map>(Map(path));
        m_maps[path] = map;
        return map;

    }

}//asteroids