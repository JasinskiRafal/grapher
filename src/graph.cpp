#include <algorithm>

#include "cli.hpp"
#include "graph.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


////////////////////////////////////
// Graph implementation
////////////////////////////////////
Graph::Graph(std::pair<std::string, std::vector<std::string>> field)
    :m_fieldname(field.first), m_fieldvalues(field.second)
{
    // Populate our internal representation of value and scale 
    this->parseValues(m_fieldvalues);
}
Graph::Graph() {}

void Graph::draw() const
{
    // Draw our data
    // TODO use matplotlib-cpp library here
}

std::vector<int> Graph::getXvalues() const
{
    return m_x_values;
}
std::vector<float> Graph::getYvalues() const
{
    return m_y_values;
}

std::string Graph::getFieldname() const
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
    m_x_values = x_buf;
    m_y_values = y_buf;

    // Note: Scale is hard coded to 1 for the meantime
    // Find domain min, max, and scale 
    m_domain.min = *min_element(m_x_values.begin(), m_x_values.end());
    m_domain.max = *max_element(m_x_values.begin(), m_x_values.end());
    m_domain.scale = 1;

    // Find range min, max, and scale
    m_range.min = *min_element(m_y_values.begin(), m_y_values.end());
    m_range.max = *max_element(m_y_values.begin(), m_y_values.end());
    m_range.scale = 1;

}

////////////////////////////////////
// GraphGroup implementation
////////////////////////////////////

// Copy constructor
GraphGroup::GraphGroup(const GraphGroup& gGroup)
{
    m_graphs = gGroup.m_graphs;
}

// Construct group from a single graph 
GraphGroup::GraphGroup(const Graph& g)
{
    m_graphs.push_back(g);
}

GraphGroup::GraphGroup() { }

void GraphGroup::expandScale(graphBounds new_domain, graphBounds new_range)
{
    // Expands domain and range if needed

    // See if the new graph has a lower/higher domain than ours
    m_domain.min = std::min(new_domain.min, m_domain.min);
    m_domain.max = std::max(new_domain.max, m_domain.max);

    // See if the new graph has a lower/higher range than ours
    m_range.min = std::min(new_range.min, m_range.min);
    m_range.max = std::max(new_range.max, m_range.max);

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

void GraphGroup::draw() const
{
    // Draw each graph in the group
    for(auto const& g : m_graphs)
    {
        g.draw();
    }
}

//  Add graph into the group
GraphGroup GraphGroup::operator+=(const Graph& rhs)
{ 
    this->m_graphs.push_back(rhs);
    // Expand our scale if needed
    this->expandScale(rhs.getDomain(), rhs.getRange());
    return (*this);
}

std::vector<Graph> GraphGroup::getGraphs() const
{
    return m_graphs;
}

graphBounds GraphGroup::getDomain() const
{
    return m_domain;
}

graphBounds GraphGroup::getRange() const
{
    return m_range;
}

GraphGroup GraphGroup::operator+=(const GraphGroup& rhs)
{ 
    //  Add another graph group into the group

    // Insert the rhs vector onto our member vector
    const std::vector<Graph> rhs_graphs = rhs.getGraphs();
    this->m_graphs.insert(m_graphs.end(),
                          rhs_graphs.begin(), rhs_graphs.end());
    // Expand our scale if needed
    this->expandScale(rhs.getDomain(), rhs.getRange());
    return (*this);
}
