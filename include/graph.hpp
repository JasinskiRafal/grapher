#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string_view>

// A graph only graphs a single field and its values
class Graph
{
    public:
        Graph(std::pair<std::string_view, std::vector<std::string_view>>
              field);
        Graph();

        //                    tuple<min, max, scale>
        void adjustScale(std::tuple<int, int, float> domain,
                         std::tuple<int, int, float> range);
        void parseValues(std::vector<std::string_view> fieldvalues);
        std::string_view getFieldname();
        std::vector<int>   getX();
        std::vector<float> getY();
    private:
        std::vector<int>   m_x;
        std::vector<float> m_y;
        std::string_view               m_fieldname;
        std::vector<std::string_view>  m_fieldvalues;
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
        void removeGraph(std::string_view field);
        // Grabs all of the fields in the group
        std::vector<std::string_view> getAllFieldnames();

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