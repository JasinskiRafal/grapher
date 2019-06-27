#ifndef CLI_HPP
#define CLI_HPP

#include <ncurses.h>
#include <iostream>
#include <csignal>

#include "csv_logdb.hpp"
#include "graph.hpp"

class CLI
{
    public:
        std::vector<std::string> getFieldsFromUser(std::vector<std::string> fields);
        void drawMainWindow();
        void addGraph(Graph g);
        void removeGraph(Graph g);

    private:
        void updateMatchedFields(std::string userInput,
                                std::vector<std::string> fields,
                                std::vector<std::string>& matchedFields);
        void removeField(std::vector<std::string>& fields,
                         std::string fieldToRemove);
        void printFields(WINDOW* stdscr, std::string userInput,
                         std::vector<std::string> matchedFields,
                         std::vector<std::string> pickedFields);
        std::vector<Graph> m_graphs;
};

// Combines multiple graph groups, each with independent axes
class Window
{
    public:
        Window(const GraphGroup& gGroup);
        Window();
        void removeGraphGroup(int groupIdx);
        Window operator+=(const GraphGroup& rhs);
        void draw();
    private:
        std::vector<GraphGroup> m_graphGroups;
};

#endif