#include "cli.hpp"

void drawWindow(Window w)
{
    w.draw();
}


////////////////////////////////////
// Window implementation
////////////////////////////////////

// Construct a window from some fields and the log database
Window::Window(std::vector<std::string> fields, LogDatabase& logDb)
{
    m_graphGroups = {GraphGroup(fields, logDb)};
}

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