#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string>

// A graph only graphs a single field and its values
class Graph
{
    public:
        Graph(std::pair<std::string, std::vector<std::string>>
              field);
        Graph();

        //                    tuple<min, max, scale>
        void adjustScale(std::tuple<int, int, float> domain,
                         std::tuple<int, int, float> range);
        void parseValues(std::vector<std::string> fieldvalues);
        std::string getFieldname();
        std::vector<int>   getX();
        std::vector<float> getY();
    private:
        std::vector<int>   m_x;
        std::vector<float> m_y;
        std::string               m_fieldname;
        std::vector<std::string>  m_fieldvalues;
};

// Combines multiple graphs, shares axes
class GraphGroup
{
    public:
        GraphGroup(const GraphGroup& gGroup);
        GraphGroup(const Graph& g);
        GraphGroup();
        //                    tuple<min, max, scale>
        void adjustScale(std::tuple<int, int, float> domain,
                         std::tuple<int, int, float> range);
        void removeGraph(std::string field);
        const std::vector<Graph> getGraphs() const;
        // Grabs all of the fields in the group
        std::vector<std::string> getAllFieldnames();

        //  Add fields from a graph into this one
        GraphGroup operator+=(const Graph& rhs);
        //  Add the graphs from another graph group into this one
        GraphGroup operator+=(const GraphGroup& rhs);

    private:
        std::vector<int> m_shared_x;
        std::vector<float> m_shared_y;
        std::vector<Graph> m_graphs;
};

#endif