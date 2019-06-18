#ifndef CLI_HPP
#define CLI_HPP

#include <ncurses.h>
#include <iostream>
#include <csignal>

#include "csv_logdb.hpp"

class GrapherCLI
{
    public:
        std::vector<std::string> getFieldsFromUser(std::vector<std::string> fields);

    private:
        void updateMatchedFields(std::string userInput,
                                std::vector<std::string> fields,
                                std::vector<std::string>& matchedFields);
        void removeField(std::vector<std::string>& fields,
                         std::string fieldToRemove);
        void printFields(WINDOW* stdscr, std::string userInput,
                         std::vector<std::string> matchedFields,
                         std::vector<std::string> pickedFields);
};

#endif