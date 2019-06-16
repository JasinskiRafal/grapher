#include <ncurses.h>
#include <iostream>
#include <csignal>

#include "csv_logdb.hpp"

void signalHandler(int signum)
{
    endwin();
    exit(signum);
}

void updateMatchedFields(std::string userInput,
                         std::vector<std::string> fields,
                         std::vector<std::string>& matchedFields)
{
    matchedFields.clear(); // Start from scratch
    for (int i = 0; i < fields.size(); i++)
    {
        std::size_t found = fields[i].find(userInput);
        // If userInput is a substring of a fields, it's a possible choice
        if (found != std::string::npos)
            matchedFields.push_back(fields[i]);
    }
}

void removeField(std::vector<std::string>& fields, std::string fieldToRemove)
{
    for (auto it = fields.begin(); it != fields.end();)
    {
        if (*it == fieldToRemove)
        {
            fields.erase(it);
            return;
        }
        else
        {
            it++;
        }
    }
}

void printFields(WINDOW* stdscr,
                 std::string userInput,
                 std::vector<std::string> matchedFields,
                 std::vector<std::string> pickedFields)
{
    addstr("Available fields:\n");
    for (auto const& f : matchedFields)
    {
        // Print out all the choices
        addstr((f + "\n").c_str());
    }
    addstr("---------\n");
    addstr("Selected:\n");
    for (auto const& f : pickedFields)
    {
        // Print out all the choices
        addstr((f + "\n").c_str());
    }
    addstr("---------\n");
    addstr((userInput + "\n").c_str());
}

std::vector<std::string> getFieldsFromUser(std::vector<std::string> fields)
{
    signal(SIGINT, signalHandler);
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    int ch;
    std::string userInput;
    // Empty input matches all fields
    std::vector<std::string> matchedFields = fields;
    // Fields user hasn't picked
    std::vector<std::string> unpickedFields = fields;
    // User has picked these
    std::vector<std::string> pickedFields;
    printFields(stdscr, userInput, matchedFields, pickedFields);
    for (;;)
    {
        // Let the user narrow down the fields until
        // one is left
        ch = getch();
        switch(ch) 
        {
            case KEY_BACKSPACE:
            case 127:
                if (userInput.size() > 1)
                    userInput.pop_back();
                clear();
                updateMatchedFields(userInput, unpickedFields, matchedFields);
                break;
            case KEY_ENTER:
            case 10:
                if (userInput.empty())
                {   // User is done making choices
                    clear();
                    endwin();
                    return pickedFields;
                }
                else if(matchedFields.size() == 1)
                {
                    // User chose a field
                    pickedFields.push_back(matchedFields.front());
                    // Remove it from our unpicked fields
                    removeField(unpickedFields, pickedFields.back());
                    userInput.clear(); // Clear our input
                    matchedFields = unpickedFields;
                    clear();
                }
                else
                {
                    // Invalid choice
                    userInput.clear();
                    matchedFields = unpickedFields; // Our input is clear
                    clear();
                }
            default:
                userInput += static_cast<char>(ch);
                clear();
                updateMatchedFields(userInput, unpickedFields, matchedFields);
                break;
        }
        // Print every cycle
        printFields(stdscr, userInput, matchedFields, pickedFields);
    } // End of for (;;)

}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage driver <file.csv>" << std::endl;
        return 0;
    }
    // Take the file as input
    std::string file(argv[1]);
    std::cout << "Parsing file:" << file << std::endl;
    LogDatabase logDb = LogDatabase(file);
    std::vector<std::string> allFields = logDb.getFields();
    std::vector<std::string> pickedFields;
    pickedFields = getFieldsFromUser(allFields);

    return 0;
}