/**
 * @file 2DMap.cpp
 * @author Henrik Gerdes (hegerdes@uni-osnabrueck.de)
 * @brief 
 * @version 0.1
 * @date 2019-03-19
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#include "CreateMap.hpp"

namespace asteroids
{
CreateMap::CreateMap(std::string mapfile):m_planets(25)
{
    using namespace boost;
    try
    {
        //Filepointer
        boost::filesystem::path fileName(mapfile);
        boost::filesystem::ifstream mapfilestream(fileName);
        
        //Check file
        if (!mapfilestream)
        {
            std::cerr << "No map file" << std::endl;
            //TODO
            //Exception?
            return;
        }

        //Setup graph
        // typedef property_map<Graph, edge_weight_t>::type star_dist_map_t;
        // star_dist_map_t star_dist = get(edge_weight, g);

        // typedef graph_traits <Graph>::vertex_descriptor Vertex;
        // typedef std::map <std::string, Vertex> NameVertexMap;
        // NameVertexMap stars;

        //Get number of elements
        std::string readline;
        std::getline(mapfilestream, readline);
        int number_of_vertices;

        std::istringstream instring(readline);
        instring >> number_of_vertices;

        //Output for testing
        std::cout << number_of_vertices << std::endl;

        //Read Nodes
        for (int i = 0; i < number_of_vertices; i++)
        {
            std::getline(mapfilestream, readline);
            std::string strname;
            int x, y, z;
            std::istringstream instring(readline);
            instring >> strname >> x >> y >> z;

            //Create Map with all planets
            m_planets[i] = std::make_shared<Planet>(Planet(strname, Vector3f(x,y,z)));
            
            //Nodes for Graph
            m_nodes.push_back(Vector3f(x,y,z));
            m_planat_dir.insert(std::pair<std::string,int>(strname, i));
        }

        //Read Edges
        for (std::string readline; std::getline(mapfilestream, readline);)
        {
            std::istringstream instring(readline);
            int pair1, pair2;
            instring >> pair1 >> pair2;

            
            //float distance = m_nodes[pair1].dist2d(m_nodes[pair2]);

            //graph_traits<Graph>::edge_descriptor e;
            // Graph::edge_descriptor e;

            // bool inserted;
            // boost::tie(e, inserted) = add_edge(start, end, g);
            // if (inserted)
            //
            //     star_dist[e] = distance;
            // }
        }

        
    }
    catch (filesystem::filesystem_error &e)
    {
        std::cout << "Can not read form PathFile" << std::endl;
        //TODO 
    }
}



CreateMap::VecPtr CreateMap::getPlanets()
{
    return m_planets;
}

void CreateMap::print()
{
    std::map<std::string,int>::iterator it;

    for(it = m_planat_dir.begin(); it !=m_planat_dir.end(); ++it)
    {
        std::cout << "Num: " << std::setw(2) << it->second << " Name: " 
            << std::left << std::setw(12) << it->first << " " << std::right 
            << std::setw(5) << std::endl;

            m_nodes[it->second].printVector();
    }
}
}
