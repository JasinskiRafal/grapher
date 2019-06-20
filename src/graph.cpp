#include "cli.hpp"


Graph::Graph() {}
Graph::Graph(std::unordered_map<std::string, std::vector<std::string>> fieldMap)
{
    this->m_fieldMap = fieldMap;
}
void Graph::adjustScale(std::tuple<int, int, float> domain,
                    std::tuple<int, int, float> range)
{
    // Adjust according to the environment variables $LINES and $COLS
}
void Graph::addFields(std::unordered_map<std::string, std::vector<std::string>> fields)
{

}
void Graph::removeFields(std::vector<std::string> fields)
{ 

}
void Graph::doInterpolation()
{
    // Draw lines between points
}
void Graph::undoInterpolation()
{ 
    // Erase lines between points
}
void Graph::zoom(float scale)
{ 

}
void Graph::unzoom(float scale)
{

}
void Graph::pan(int x, int y)
{ 

}
void Graph::reset()
{ 

}
// Create graph using fields from both graphs 
Graph Graph::operator+(const Graph& rhs)
{ 

}
//  Add fields from a graph into this one
Graph Graph::operator+=(const Graph& rhs)
{ 

}

void Graph::makeDefaultScale()
{ 

}
void Graph::draw()
{ 
    // Fill out a buffer that can then be sent to ncurses
    // Using braille characters might be a good idea
}