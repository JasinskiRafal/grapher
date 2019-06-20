#include "cli.hpp"

void signalHandler(int signum)
{
    endwin();
    exit(signum);
}

/////////////////////////////////////////////
// CLI implemenation
/////////////////////////////////////////////
void CLI::updateMatchedFields(std::string userInput,
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

void CLI::removeField(std::vector<std::string>& fields, std::string fieldToRemove)
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

void CLI::printFields(WINDOW* stdscr, std::string userInput,
                             std::vector<std::string> matchedFields,
                             std::vector<std::string> pickedFields)
{
    addstr("Available fields:\n");
    for (auto const& m : matchedFields)
    {
        // Print out all the choices
        addstr((m + "\n").c_str());
    }
    addstr("---------\n");
    addstr("Selected:\n");
    for (auto const& p : pickedFields)
    {
        // Print out all the choices
        addstr((p + "\n").c_str());
    }
    addstr("---------\n");
    addstr((userInput + "\n").c_str());
}

std::vector<std::string> CLI::getFieldsFromUser(std::vector<std::string> fields)
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
                if (userInput.size() > 0)
                    userInput.pop_back();
                updateMatchedFields(userInput, unpickedFields, matchedFields);
                break;
            case KEY_ENTER:
            case 36:
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
                    removeField(unpickedFields, matchedFields.front());
                    userInput.clear(); // Clear our input
                    matchedFields = unpickedFields;
                }
                else
                {
                    // Invalid choice
                    userInput.clear();
                    matchedFields = unpickedFields; // Our input is clear
                }
                break;
            default:
                userInput += static_cast<char>(ch);
                updateMatchedFields(userInput, unpickedFields, matchedFields);
                break;
        } // End of switch(ch)

				clear();
        // Print every cycle
        printFields(stdscr, userInput, matchedFields, pickedFields);

    } // End of for (;;)

}
