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
class Map
{
  public:
    //Pointer to map
    using Ptr = std::shared_ptr<Map>;
    using VecPtr = std::vector<Planet::Ptr>;
    
    //Define Graph
    typedef adjacency_list <listS, vecS, undirectedS, no_property, property < edge_weight_t, float > > Graph;

    /**
     * @brief Construct a new Map object
     * 
     * @param mapfile 
     */
    Map(std::string mapfile);

    /**
     * @brief Get the Planets object
     * 
     * @return VecPtr Returns a shared pointer that holds all Planets
     */
    VecPtr& getPlanets();

    /**
     * @brief Get a list with Vector3f. This is the shortest path between planets
     * 
     * @param position  Current position
     * @param start_planet Number of start planet
     * @param end_planet  Number of end planet
     * @return std::list<Vector3f> The path
     */
    std::list<int> getPath(int start_planet, int end_planet);

    /**
     * @brief getEdges returns all edges
     * @return a vector of int pairs representing the planet ids which are connected
     */
    std::vector<Vector2i> getEdges() {return m_edges;}

    /**
     * @brief Get the Number Of Planets object
     * 
     * @return int Number of planets
     */
    int getNumberOfPlanets(){return m_num_of_planets;}

    /**
     * @brief Prints every planet with pos
     * 
     */
    void printPlanets();

    /**
     * @brief print edges;
     * 
     */
    void printEdges();

    /**
     * @brief Print nodes
     * 
     */
    void printNodes();

  private:

    //The graph
    Graph g;

    //Planets
    VecPtr m_planets;

    //Edges
    std::vector<Vector2i> m_edges;

    //Nodes
    std::vector<int> m_nodes;

    //Map to match the Names to a Index
    std::map<std::string,int> m_planat_dir;

    //Number of planets
    int m_num_of_planets;

};

}//asteroids

#endif
