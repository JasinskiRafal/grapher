#include "cli.hpp"

////////////////////////////////////
// Window implementation
////////////////////////////////////
Window Window::operator+=(const GraphGroup& rhs)
{
    m_graphGroups.push_back(rhs);
    return *this;
}

void Window::removeGraphGroup(int groupIdx)
{

}

void Window::draw()
{

}