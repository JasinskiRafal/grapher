#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>
#include <string>

#include "csv_logdb.hpp"
#include "gnuplot-iostream.h"

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

        std::string getFieldname() const;
        std::vector<int>   getXvalues() const;
        std::vector<float> getYvalues() const;
        graphBounds        getDomain() const;
        graphBounds        getRange() const;
        // Used to stream graph data to gnuplot-iostream
        //Gnuplot& operator<<(Gnuplot& os);
        void parseValues(std::vector<std::string> fieldvalues);
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


// Combines multiple graphs, shares axes
class GraphGroup
{
    public:
        GraphGroup(std::vector<std::string> fields, LogDatabase& logDb);
        GraphGroup(const GraphGroup& gGroup);
        GraphGroup(const Graph& g);
        GraphGroup();

        void expandScale(graphBounds new_domain, graphBounds new_range);
        void removeGraph(std::string field);
        void draw(Gnuplot& gp) const;
        std::vector<Graph> getGraphs() const;
        // Grabs all of the fields in the group
        std::vector<std::string> getAllFieldnames() const;
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


// Combines multiple graph groups, each with independent axes
class Window
{
    public:
        Window(const GraphGroup& gGroup);
        Window(std::vector<std::string> fields, LogDatabase& logDb);
        Window();
        void removeGraphGroup(int groupIdx);
        Window operator+=(const GraphGroup& rhs);
        void draw();
    private:
        std::vector<GraphGroup> m_graphGroups;
};

// Non member function used for spawning a thread
void drawWindow(Window w);


#endif