#include "cli.hpp"

void signalHandler(int signum)
{
    endwin();
    exit(signum);
}

/////////////////////////////////////////////
// CLI implemenation
/////////////////////////////////////////////

CLI::CLI(const LogDatabase logDb) 
    : m_allFields(logDb.getFieldnames()), m_fieldMap(logDb.getFieldMap())
{
    m_pickedFields = {};
    m_unpickedFields.reserve(m_allFields.size());

    // All of the fields are unpicked on construction
    std::transform(m_allFields.begin(), m_allFields.end(),
                   back_inserter(m_unpickedFields),
    [] (std::string const & field)
        {
            // Transform each std::string to a string_view
            return field.c_str();
        });
    // We have no user input yet, so all the unpicked fields can be matched
    // to the empty string
    m_matchedFields = m_unpickedFields;
}

std::vector<std::string_view> CLI::fuzzySearchFields(std::string userInput)
{
    // This function will return all the fields in <fields> if <userInput> is
    // a substring of a field in <fields>
    std::vector<std::string_view> searchResults;

    for (int i = 0; i < m_unpickedFields.size(); i++)
    {
        // Search this fieldname with userInput as a substring
        std::size_t found = m_unpickedFields[i].find(userInput);
        if (found != std::string::npos)
        {
            // userInput is a substring of a fields, it's a possible choice
            searchResults.push_back(m_unpickedFields[i]);
        }
    }

    return searchResults;
}

void CLI::removeField(std::vector<std::string_view>& fields,
                      std::string_view fieldToRemove)
{
    for (auto it = fields.begin(); it != fields.end();)
    {
        // We are compaing a string_view to string
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

void CLI::printFields(WINDOW* stdscr, std::string userInput) const
{
    addstr("Available fields:\n");
    for (auto const& m : m_matchedFields)
    {
        // Print out all the choices
        addstr(m.data());
        addstr("\n");
    }
    addstr("---------\n");
    addstr("Selected:\n");
    for (auto const& p : m_pickedFields)
    {
        // Print out all the choices
        addstr(p.data());
        addstr("\n");
    }
    addstr("---------\n");
    addstr((userInput + "\n").c_str());
}

FieldMap CLI::getFieldsFromUser()
{
    signal(SIGINT, signalHandler);
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    cbreak();
    int ch;
    std::string userInput;
    printFields(stdscr, userInput);
    for (;;)
    {
        // Let the user narrow down the fields until
        // one is left
        ch = getch();
        switch(ch) 
        {

            case KEY_BACKSPACE:
            case 127:
            /* User has removed a character -----*/
                if (userInput.size() > 0)
                    userInput.pop_back();
                m_matchedFields = fuzzySearchFields(userInput);
                break;
            case KEY_ENTER:
            case 36:
            case 10:
            /* User has pressed enter ---------- */
                if (userInput.empty())
                {   // User is done making choices
                    clear();
                    endwin();
                    return fieldsToFieldMap(m_pickedFields);
                }
                else if(m_matchedFields.size() == 1)
                {
                    // User chose a field
                    m_pickedFields.push_back(m_matchedFields.front());
                    // Remove it from our unpicked fields
                    removeField(m_unpickedFields, m_matchedFields.front());
                    userInput.clear(); // Clear our input
                    m_matchedFields = m_unpickedFields;
                }
                else
                {
                    // Invalid choice
                    userInput.clear();
                    m_matchedFields = m_unpickedFields; // Our input is clear
                }
                break;
            default:
            /* User has pressed any key other
               than ENTER or BACKSPACE ---------- */
                userInput += static_cast<char>(ch);
                m_matchedFields = fuzzySearchFields(userInput);
                break;
        } // End of switch(ch)

        clear();
        // Print every cycle
        printFields(stdscr, userInput);

    } // End of for (;;)

}

FieldMap CLI::fieldsToFieldMap(const std::vector<std::string_view> fields)
{
    FieldMap fm;
    for (const std::string_view s_view : fields)
    {
        // Need to convert string_view to string when using the FieldMap
        std::string s(s_view);
        fm[s] = m_fieldMap.at(s);
    }

    return fm;
}