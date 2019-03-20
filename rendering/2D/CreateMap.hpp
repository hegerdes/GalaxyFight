/**
 * @file 2DMap.hpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief Manages map creation. 
 *        Reads file from drive, creates map with all planets
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef MAP_HPP
#define MAP_HPP

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iomanip>
#include <iostream>
#include <map>
#include <list>
#include <math.h>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/astar_search.hpp>
#include <boost/graph/random.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/random.hpp>
#include <boost/config.hpp>

#include "Planet.hpp"

using namespace boost;

namespace asteroids
{
class CreateMap
{
  public:
    //Pointer to map
    using MapPtr = std::shared_ptr<std::map<int,Planet>>;
    using VecPtr = std::vector<Planet::Ptr>;
    
    //Define Graph
    typedef adjacency_list <listS, vecS, undirectedS, no_property, property < edge_weight_t, float > > Graph;

    /**
     * @brief Construct a new Map object
     * 
     * @param mapfile 
     */
    CreateMap(std::string mapfile);

    /**
     * @brief Get the Planets object
     * 
     * @return VecPtr Returns a shared pointer that holds all Planets
     */
    VecPtr getPlanets();

    /**
     * @brief Prints every planet with pos
     * 
     */
    void print();

  private:

    //The graph
    Graph g;

    //Planets
    VecPtr m_planets;

    //Edges
    std::vector<Vector2i> m_edges;

    //Nodes
    std::vector<Vector3f> m_nodes;

    //Map to match the Names to a Index
    std::map<std::string,int> m_planat_dir;




};


}//asteroids

#endif