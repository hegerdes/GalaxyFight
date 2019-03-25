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


#include "Map.hpp"

namespace asteroids
{

// heuristic for graph
template<class Graph, class CostType, class LocMap>
class distance_heuristic : public astar_heuristic<Graph, CostType>
{
public:
	typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    distance_heuristic(LocMap l, Vertex goal, Map::VecPtr planets) :
		m_location(l), m_goal(goal)
	{
        m_boost_planets = planets;
	}
	CostType operator()(Vertex u)
    {
        return m_boost_planets[m_location[u]]->getPos().dist2d(m_boost_planets[m_location[u]]->getPos());

	}
private:
    Map::VecPtr m_boost_planets;
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

Map::Map(std::string mapfile):m_planets(17)
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
        m_num_of_planets = number_of_vertices;

        //Read Nodes
        for (int i = 0; i < number_of_vertices; i++)
        {
            std::getline(mapfilestream, readline);
            std::string strname;
            int x, y, z;
            std::istringstream instring(readline);
            instring >> strname >> x >> y >> z;

            //Create Map with all planets
            m_planets[(unsigned long)i] = std::make_shared<Planet>(Planet(i, strname, Vector3f(x,y,z)));
            
            //Nodes for Graph
            m_nodes.push_back(i);
            m_planat_dir.insert(std::pair<std::string,int>(strname, i));
        }

        //Read Edges
        for (std::string readline; std::getline(mapfilestream, readline);)
        {
            std::istringstream instring(readline);
            int pair1, pair2;
            instring >> pair1 >> pair2;

            //create edge list
            m_edges.push_back(Vector2i(pair1, pair2));
            
            float distance = m_planets[(unsigned long) pair1]->getPos().dist2d(m_planets[(unsigned long)pair2]->getPos());

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

Map::VecPtr& Map::getPlanets()
{
    return m_planets;
}

std::list<int> Map::getPath(int start, int goal)
{
    std::cout << "Start: " << start  << " " <<  m_planets[start]->getname() 
        << "\n" << "End: " << goal << " " << m_planets[goal]->getname() << std::endl;

    typedef Graph::vertex_descriptor vertex;

    std::vector<Graph::vertex_descriptor> p(num_vertices(g));
    std::vector<float> d(num_vertices(g));

    try {
      //call astar named parameter interface
        astar_search(g, start, distance_heuristic<Graph, float, std::vector<int> >
            (m_nodes, goal, m_planets),
            predecessor_map(&p[0]).distance_map(&d[0]).
            visitor(star_graph_visitor<vertex>(goal)));
    
    
    } catch(found_goal fg)
    {
        std::cout << "Found Goal" << std::endl; 
        std::list<int> shortest_path;
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
    std::list<int> solutionPath;
    return solutionPath;
}

void Map::printPlanets()
{
    for (const auto& planet : m_planets) 
    {
        //print every pnlanet DEBUG
        planet->printPlanet();
    }
}

void Map::printEdges()
{
    std::vector<Vector2i>::iterator it;

    std::cout << "Print Edges" << std::endl; 
    for(it = m_edges.begin(); it !=m_edges.end(); ++it)
    {
        it->printVector();
    }
}

void Map::printNodes()
{
    std::cout << "Print Nodes" << std::endl;
     for(auto it = m_nodes.begin(); it !=m_nodes.end(); ++it)
     {
         std::cout << "PlanetID: " << *(it) << std::endl;
     }
}
}
