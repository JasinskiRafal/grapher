#include "cli.hpp"

////////////////////////////////////
// Window implementation
////////////////////////////////////

// Copy constructor
Window::Window(const GraphGroup& gGroup)
{
    m_graphGroups = {gGroup};
}

Window::Window()
{
}

Window Window::operator+=(const GraphGroup& rhs)
{
    m_graphGroups.push_back(rhs);
    return *this;
}

void Window::removeGraphGroup(int groupIdx)
{
    m_graphGroups.erase(m_graphGroups.begin() + groupIdx);
}

void Window::draw()
{
    // Call on each of the groups to draw their graphs
    for (auto const& gGroup : m_graphGroups)
    {
        gGroup.draw();
    }
}