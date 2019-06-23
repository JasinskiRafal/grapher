#include <algorithm>
#include <charconv>

#include "cli.hpp"
#include "graph.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


////////////////////////////////////
// Graph implementation
////////////////////////////////////
Graph::Graph(std::pair<std::string, std::vector<std::string>> field)
    :m_fieldname(field.first), m_fieldvalues(field.second) { }
Graph::Graph() {}

std::vector<int>   Graph::getX() { return m_x;}
std::vector<float> Graph::getY() { return m_y;}

void Graph::adjustScale(std::tuple<int, int, float> domain,
                        std::tuple<int, int, float> range)
{
}

std::string Graph::getFieldname()
{
    return m_fieldname;
}

void Graph::parseValues(std::vector<std::string> fieldvalues)
{
    // Buffer input so we don't add to our class variables if we get
    // an exception
    std::vector<float> y_buf(fieldvalues.size());
    std::vector<int>   x_buf(fieldvalues.size());
    // Fill x axis with ascending values
    std::iota(x_buf.begin(), x_buf.end(), 0);
    std::string::size_type size;
    float y_max = 0.0, y_min = 0.0;

    for (auto const& str: fieldvalues)
    {
        // Try to parse field value
        try {
            y_buf.push_back(stof(str, &size));
        } catch (std::invalid_argument e) {
            return; // Could not parse float
        }
        
    }

    // Parsing has succeeded
    m_x = x_buf;
    m_y = y_buf;

}

////////////////////////////////////
// GraphGroup implementation
////////////////////////////////////
GraphGroup::GraphGroup(const GraphGroup& gGroup)
{
    m_graphs   = gGroup.m_graphs;
    m_shared_x = gGroup.m_shared_x;
    m_shared_y = gGroup.m_shared_y;
}

GraphGroup::GraphGroup(const Graph& g)
{
    m_graphs.push_back(g);
    m_shared_x = m_graphs.front().getX();
    m_shared_y = m_graphs.front().getY();
}

GraphGroup::GraphGroup() { }

//  Add graphs into the group
GraphGroup GraphGroup::operator+=(const Graph& rhs)
{ 
    this->m_graphs.push_back(rhs);
    return *this;
}

const std::vector<Graph> GraphGroup::getGraphs() const
{
    return m_graphs;
}
//  Add another graph group into the group
GraphGroup GraphGroup::operator+=(const GraphGroup& rhs)
{ 
    // Insert the rhs vector onto our member vector
    const std::vector<Graph> rhs_graphs = rhs.getGraphs();
    this->m_graphs.insert(m_graphs.end(),
                          rhs_graphs.begin(), rhs_graphs.end());
    return *this;
}

void GraphGroup::removeGraph(std::string field)
{
    // Search for field in our graphs, remove it if it exists
    for (auto it = m_graphs.begin(); it != m_graphs.end();)
    {
        if ((*it).getFieldname() == field)
        {
            m_graphs.erase(it);
        }
    }
}
