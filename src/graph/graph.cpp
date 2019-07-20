#include <algorithm>
#include <numeric>
#include <utility>

#include "cli.hpp"
#include "graph.hpp"
#include "gnuplot-iostream.h"

/**
 * @brief Allows a graph object to be directly streamed to a GNU plot
 * 
 * @param[out] gp - GNUplot object to be streamed to
 * @param[in] g - Graph the data is taken from 
 * @return Gnuplot&
 */
Gnuplot& operator<<(Gnuplot& gp, const Graph& g)
{
    //  Add our values to the plot
//    gp << "plot" << gp.file1d(g.getYvalues()) << " with lines title '" +
//         g.getFieldname() + "'" << std::endl;
    gp << gp.file1d(g.getYvalues()) << " with lines title '" +
         g.getFieldname() + "'";
    return gp;
}

////////////////////////////////////
// Graph implementation
////////////////////////////////////

/** 
 * @brief Constructs a graph given a single field and its info
 * @param[in] field - A pair of a field and its values (as strings)
 */
Graph::Graph(const FieldPair& field)
{
    // Populate our internal representation of value and scale 
    m_fieldname = field.first;
    m_fieldvalues = field.second;
    this->parseValues(m_fieldvalues);
}
/**
 * @brief Default constructor
 */
Graph::Graph() {}

/**
 * @brief Get the values on the x axis
 * 
 * @return std::vector<int>
 */
std::vector<int> Graph::getXvalues() const
{
    return m_x_values;
}

/**
 * @brief Get the values on the y axis
 * 
 * @return std::vector<float> 
 */
std::vector<float> Graph::getYvalues() const
{
    return m_y_values;
}

/**
 * @brief Get the domain, anything outside of this on the
 * x axis will not be graphed.
 * 
 * @return graphBounds - Max, min, and scale
 */
graphBounds Graph::getDomain() const
{
    return m_domain;
}

/**
 * @brief Get the range, anything outside of this on the
 * y axis will not be graphed.
 * 
 * @return graphBounds - Max, min, and scale
 */
graphBounds Graph::getRange() const
{
    return m_range;
}

/**
 * @brief Get the name of the graphed field
 * 
 * @return std::string 
 */
std::string Graph::getFieldname() const
{
    return m_fieldname;
}

/**
 * @brief Convert the values of the field to the y axis. Assumes
 * the x values to be ascending integers
 * 
 * @param[in] fieldvalues - values of the field as strings
 */
void Graph::parseValues(const std::vector<std::string>& fieldvalues)
{
    // Buffer input so we don't add to our class variables if we get
    // an exception
    // We will be iteratively appending to this
    std::vector<float> y_buf = {};

    // Fill x axis with ascending values
    std::vector<int>   x_buf(fieldvalues.size());
    std::iota(x_buf.begin(), x_buf.end(), 0);

    std::string::size_type size;
    float y_max = 0.0, y_min = 0.0;

    for (auto const& str: fieldvalues)
    {
        // Try to parse field value
        try {
            y_buf.push_back(stof(str, &size));
        } catch (std::invalid_argument e) {
            std::cout << "Could not parse:" << str << std::endl;
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

/**
 * @brief Construct a group of graphs with each field mapping
 * to a new graph
 * 
 * @param[in] fieldMap - mapping of fields to a list of values
 */
GraphGroup::GraphGroup(const FieldMap& fieldMap)
{
    for (auto const& pair : fieldMap)
    {
        // Construct a graph from the field,fieldvalues pair
        m_graphs.push_back(Graph(pair));
        // Adjust the shared scale
        this->expandScale(m_graphs.back().getDomain(),
                          m_graphs.back().getRange());
    }
}

/**
 * @brief Copy constructor
 * 
 * @param[in] gGroup
 */
GraphGroup::GraphGroup(const GraphGroup& gGroup)
{
    m_graphs = gGroup.m_graphs;
    this->expandScale(gGroup.getDomain(), gGroup.getDomain());
}

/**
 * @brief Construct a graph group from a single graph
 * 
 * @param g - The single graph
 */
GraphGroup::GraphGroup(const Graph& g)
{
    m_graphs = {g};
    this->expandScale(g.getDomain(), g.getDomain());
}

/**
 * @brief Default constructor
 * 
 */
GraphGroup::GraphGroup() { }

/**
 * @brief Increases the domain and range if the new domain and
 * range are larger
 * 
 * @param[in] new_domain 
 * @param[in] new_range 
 */
void GraphGroup::expandScale(const graphBounds new_domain,
                             const graphBounds new_range)
{
    // Expands domain and range if needed

    // See if the new graph has a lower/higher domain than ours
    m_domain.min = std::min(new_domain.min, m_domain.min);
    m_domain.max = std::max(new_domain.max, m_domain.max);

    // See if the new graph has a lower/higher range than ours
    m_range.min = std::min(new_range.min, m_range.min);
    m_range.max = std::max(new_range.max, m_range.max);

}

/**
 * @brief Removes a graph from a group of graphs
 * 
 * @param[in] field - Name of the field to remove
 */
void GraphGroup::removeGraph(const std::string& field)
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

/**
 * @brief Draw every graph in a graph group via GNUplot
 * 
 * @param[in] gp - The stream to output to
 */
void GraphGroup::draw(Gnuplot& gp) const
{

    // General multiplot setup
    gp << "set title 'CSV Data'" << std::endl;

    // Allow for our data to share axes
    gp << "set multiplot" << std::endl;
    gp << "plot ";
    // Draw each graph in the group
    for(auto const& graph : m_graphs)
    {
        // Stream the data from each graph
        gp << graph << ",";
    }
    // Add a space and newline at the end
    gp << " " << std::endl;

    gp << "set mouse" << std::endl;
    // This triggers the gnuplot window to pop up
    gp << "unset multiplot" << std::endl;
}

/**
 * @brief Gets the vector of graphs from the group via copy
 * 
 * @return std::vector<Graph> 
 */
std::vector<Graph> GraphGroup::getGraphs() const
{
    return m_graphs;
}

/**
 * @brief Gets the domain of the group as a whole
 * 
 * @return graphBounds 
 */
graphBounds GraphGroup::getDomain() const
{
    return m_domain;
}

/**
 * @brief Gets the range of the group as a whole
 * 
 * @return graphBounds 
 */
graphBounds GraphGroup::getRange() const
{
    return m_range;
}

/**
 * @brief Add a single graph to the group of graphs
 * 
 * @param[in] rhs 
 * @return GraphGroup 
 */
GraphGroup GraphGroup::operator+=(const Graph& rhs)
{ 
    this->m_graphs.push_back(rhs);
    // Expand our scale if needed
    this->expandScale(rhs.getDomain(), rhs.getRange());
    return (*this);
}

/**
 * @brief Adds a group of graphs to this group of graphs
 * 
 * @param[in] rhs 
 * @return GraphGroup 
 */
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

/**
 * @brief Replaces the data of this graph with rhs
 * 
 * @param[in] rhs 
 * @return GraphGroup 
 */
GraphGroup GraphGroup::operator=(const GraphGroup& rhs)
{ 
    this->m_graphs = rhs.getGraphs();
    this->m_domain = rhs.getDomain();
    this->m_range  = rhs.getRange();

    this->expandScale(this->m_domain, this->m_range);

    return (*this);
}