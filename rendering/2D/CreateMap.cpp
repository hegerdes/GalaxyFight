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

// heuristic for graph
template<class Graph, class CostType, class LocMap>
class distance_heuristic : public astar_heuristic<Graph, CostType>
{
public:
	typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
	distance_heuristic(LocMap l, Vertex goal) :
		m_location(l), m_goal(goal)
	{
	}
	CostType operator()(Vertex u)
	{   
        //Calls Vector3f dist
        return m_location[m_goal].dist(m_location[u]);
	}
private:
	LocMap m_location;
	Vertex m_goal;
};

//For goal found
struct found_goal {};

//Terminate if goal found
template<class Vertex>
class star_graph_visitor: public boost::default_astar_visitor
{
public:
	star_graph_visitor(Vertex goal):m_goal(goal)
	{
	}

    template <class Graph>
    void examine_vertex(Vertex u, Graph& g) 
    {
        if(u == m_goal)
        {
          throw found_goal();
        }
    }
    
private:
	Vertex m_goal;
};

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
        typedef property_map<Graph, edge_weight_t>::type star_dist_map_t;
        star_dist_map_t star_dist = get(edge_weight, g);

        typedef graph_traits <Graph>::vertex_descriptor Vertex;
        typedef std::map <std::string, Vertex> NameVertexMap;
        NameVertexMap stars;

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

            
            float distance = m_nodes[pair1].dist2d(m_nodes[pair2]);

            //graph_traits<Graph>::edge_descriptor e;
            Graph::edge_descriptor e;

            bool inserted;
            boost::tie(e, inserted) = add_edge(pair1, pair2, g);
            if (inserted)
            {
                star_dist[e] = distance;
            }
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

std::list<Vector3f > CreateMap::getPath(Vector3f position, int start, int goal)
{
    std::cout << "Start: " << start  << " " <<  m_planets[start]->getname() 
        << "\n" << "End: " << goal << " " << m_planets[goal]->getname() << std::endl;

    typedef Graph::vertex_descriptor vertex;

    std::vector<Graph::vertex_descriptor> p(num_vertices(g));
    std::vector<float> d(num_vertices(g));

    try {
      //call astar named parameter interface
        astar_search(g, start, distance_heuristic<Graph, float, std::vector<Vector3f> >
            (m_nodes, goal),
            predecessor_map(&p[0]).distance_map(&d[0]).
            visitor(star_graph_visitor<vertex>(goal)));
    
    
    } catch(found_goal fg)
    {
        std::cout << "Found Goal" << std::endl; 
        std::list<Vector3f> shortest_path;
        shortest_path.push_front(position);
        for(vertex v = goal;; v = p[v]) {
            shortest_path.push_front(m_nodes[v]);
            if(p[v] == v)
            {
                break;
            }
      }
      return shortest_path;
    }

    //If there is no solution
    std::cout << "Found no Path" << std::endl;
    std::list<Vector3f > solutionPath;
    solutionPath.push_front(position);
    return solutionPath;
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
