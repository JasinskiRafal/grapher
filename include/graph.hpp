#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <unordered_map>
#include <vector>

class Graph
{
    public:
        Graph();
        Graph(std::unordered_map<std::string, std::vector<std::string>> fieldMap);
        void adjustScale(std::tuple<int, int, float> domain,
                         std::tuple<int, int, float> range);
        void addFields(std::unordered_map<std::string, std::vector<std::string>> fields);
        void removeFields(std::vector<std::string> fields);
        void doInterpolation();
        void undoInterpolation();
        void zoom(float scale);
        void unzoom(float scale);
        void pan(int x, int y);
        void reset();

        // Create graph using fields from both graphs 
        Graph operator+(const Graph& rhs);
        //  Add fields from a graph into this one
        Graph operator+=(const Graph& rhs);

    private:
        void makeDefaultScale();
        void draw();
        std::unordered_map<std::string, std::vector<std::string>> m_fieldMap;
        // TODO Make some type of matrix to manipulate the data before sending
        // to ncurses as a const char*
};

#endif