#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string>

typedef struct 
{
    int   min;
    int   max;
    float scale;
} graphBounds;

// A graph only graphs a single field and its values
class Graph
{
    public:
        Graph(std::pair<std::string, std::vector<std::string>>
              field);
        Graph();

        void adjustScale(graphBounds domain,
                         graphBounds range);
        void parseValues(std::vector<std::string> fieldvalues);
        std::string getFieldname() const;
        std::vector<int>   getXvalues() const;
        std::vector<float> getYvalues() const;
        graphBounds        getDomain() const;
        graphBounds        getRange() const;
    private:
        std::vector<int>   m_x;
        std::vector<float> m_y;
        graphBounds        m_domain;
        graphBounds        m_range;
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

        void expandScale(graphBounds new_domain, graphBounds new_range);
        void removeGraph(std::string field);
        std::vector<Graph> getGraphs() const;
        // Grabs all of the fields in the group
        std::vector<std::string> getAllFieldnames() const;
        graphBounds getDomain() const;
        graphBounds getRange() const;

        //  Add fields from a graph into this one
        GraphGroup operator+=(const Graph& rhs);
        //  Add the graphs from another graph group into this one
        GraphGroup operator+=(const GraphGroup& rhs);

    private:
        graphBounds m_domain;
        graphBounds m_range;
        std::vector<Graph> m_graphs;
};

#endif