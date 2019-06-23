#include <algorithm>
#include <charconv>

#include "cli.hpp"
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;


////////////////////////////////////
// Graph implementation
////////////////////////////////////
Graph::Graph(std::pair<std::string_view, std::vector<std::string_view>> field)
    :m_fieldname(field.first), m_fieldvalues(field.second) { }
Graph::Graph() {}

std::vector<int>   Graph::getX() { return m_x;}
std::vector<float> Graph::getY() { return m_y;}

void Graph::adjustScale(std::tuple<int, int, float> domain,
                        std::tuple<int, int, float> range)
{
}

void Graph::parseValues(std::vector<std::string_view> fieldvalues)
{
    // Buffer input so we don't add to our class variables if we get
    // an exception
    std::vector<float> y_buf(fieldvalues.size());
    std::vector<int>   x_buf(fieldvalues.size());
    // Fill x axis with ascending values
    std::iota(x_buf.begin(), x_buf.end(), 0);
    std::string::size_type size;
    float flt_point;
    float y_max = 0.0, y_min = 0.0;

    for (auto const& str: fieldvalues)
    {
        // Try to parse field value
        // TODO Use from_chars correctly someho
        if (auto [p, ec] =
         std::from_chars(str.data(), str.data()+str.size(), flt_point);
            ec == std::errc())
        {
            y_buf.push_back(flt_point);
        }
        else
        {
            return;
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

}
//  Add another graph group into the group
GraphGroup GraphGroup::operator+=(const GraphGroup& rhs)
{ 

}

void GraphGroup::removeGraph(std::string_view field)
{

}
