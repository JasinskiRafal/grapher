#include "graph.hpp"

void drawWindow(Window w)
{
    w.draw();
}


////////////////////////////////////
// Window implementation
////////////////////////////////////

/**
 * @brief Constructs a Window with pre-filled fields
 * @param[in] fieldMap - map of fields to values
 */
Window::Window(const FieldMap& fieldMap)
{
    m_graphGroups = {GraphGroup(fieldMap)};
}

/**
 * @brief Constructs a Window from another graph group 
 * @param[in] gGroup
 */
Window::Window(const GraphGroup& gGroup)
{
    m_graphGroups = {gGroup};
}

/**
 * @brief Default constructor
 * 
 */
Window::Window() { }

/**
 * @brief Replace the fields of the current window
 * 
 * @param[in] fieldMap 
 */
void Window::replaceFields(const FieldMap& fieldMap)
{
    m_graphGroups = {GraphGroup(fieldMap)};
}

/**
 * @brief Add a graph group to the window
 * 
 * @param[in] rhs 
 * @return Window 
 */
Window Window::operator+=(const GraphGroup& rhs)
{
    m_graphGroups.push_back(rhs);
    return *this;
}

/**
 * @brief Remove a graph group by index
 * 
 * @param[in] groupIdx 
 */
void Window::removeGraphGroup(const int groupIdx)
{
    m_graphGroups.erase(m_graphGroups.begin() + groupIdx);
}

/**
 * @brief Draw all of the graph groups in the window
 */
void Window::draw()
{
    // The gnuplot is shared among the graph groups in this window
    Gnuplot gp;

    // Call on each of the groups to draw their graphs
    for (auto const& gGroup : m_graphGroups)
    {
        gGroup.draw(gp);
    }

    // Let the user keep the window open as long as they want
    for (;;)
    {
        gp << "pause 1" << std::endl;
        gp << "reread" << std::endl;
    }
}