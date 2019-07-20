#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string>

#include "csv_logdb.hpp"
#include "gnuplot-iostream.h"

/**
 * @brief Used for describing the domain and range of a graph along
 * with its scaling
 */
typedef struct 
{
    /** @brief the lower bound of domain or range */
    int   min;
    /** @brief the upper bound of domain or range */
    int   max;
    /** @brief the scale of the domain or range. The space between ticks */
    float scale;
} graphBounds;
      

/**
 * @brief A graph only graphs a single field and its values
 */
class Graph
{
    public:
        Graph(const FieldPair& field);
        Graph();

        std::string getFieldname() const;
        std::vector<int>   getXvalues() const;
        std::vector<float> getYvalues() const;
        graphBounds        getDomain() const;
        graphBounds        getRange() const;
        // Used to stream graph data to gnuplot-iostream
        //Gnuplot& operator<<(Gnuplot& os);
        void parseValues(const std::vector<std::string>& fieldvalues);
    private:
        std::vector<int>   m_x_values;
        std::vector<float> m_y_values;
        graphBounds        m_domain;
        graphBounds        m_range;
        std::string               m_fieldname;
        std::vector<std::string>  m_fieldvalues;
};

// Overload for streaming graph data into gnuplot-iostream
Gnuplot& operator<<(Gnuplot& os, const Graph& g);


/**
 * @brief Combines multiple graphs, shares axes
 */
class GraphGroup
{
    public:
        GraphGroup(const FieldMap& fieldMap);
        GraphGroup(const GraphGroup& gGroup);
        GraphGroup(const Graph& g);
        GraphGroup();

        void expandScale(const graphBounds new_domain,
                         const graphBounds new_range);
        void removeGraph(const std::string& field);
        void draw(Gnuplot& gp) const;
        std::vector<Graph> getGraphs() const;
        graphBounds getDomain() const;
        graphBounds getRange() const;

        //  Add fields from a graph into this one
        GraphGroup operator+=(const Graph& rhs);
        //  Add the graphs from another graph group into this one
        GraphGroup operator+=(const GraphGroup& rhs);
        
        GraphGroup operator=(const GraphGroup& rhs);

    private:
        graphBounds m_domain;
        graphBounds m_range;
        std::vector<Graph> m_graphs;
};

/**
 * @brief Combines multiple graph groups, each with independent axes
 */
class Window
{
    public:
        Window(const GraphGroup& gGroup);
        Window(const FieldMap& fieldMap);
        Window();
        void replaceFields(const FieldMap& fieldMap);
        void removeGraphGroup(const int groupIdx);
        Window operator+=(const GraphGroup& rhs);
        void draw();
    private:
        std::vector<GraphGroup> m_graphGroups;
};

/** @brief Non member function used for spawning a thread */
void drawWindow(Window w);


#endif