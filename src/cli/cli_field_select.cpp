#include "cli.hpp"

/**
 * @brief Ends ncurses session when a <signum> signal is sent
 * 
 * @param[in] signum - number of signal to handle
 */
void signalHandler(int signum)
{
    endwin();
    exit(signum);
}

/////////////////////////////////////////////
// CLI implemenation
/////////////////////////////////////////////

/**
 * @brief Construct a command line interface to let the user interact
 * with the log database
 * 
 * @param[in] logDb - The log data to interact with
 */
CLI::CLI(const LogDatabase& logDb) 
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

/**
 * @brief Search our unpicked fields to see if <userInput> is a substring
 * of any string in the vector of unpicked fields
 * 
 * @param[in] userInput - The substring to search for
 * @return std::vector<std::string_view> - Vector of values where <userInput>
 * is a substring
 */
std::vector<std::string_view>
CLI::fuzzySearchFields(const std::string& userInput)
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

/**
 * @brief Given a vector of fields, remove a field from it
 * 
 * @param[out] fields - The vector of fields to remove <fieldToRemove> from
 * @param[in] fieldToRemove - The field to remove from <field>
 */
void CLI::removeField(std::vector<std::string_view>& fields,
                      const std::string_view& fieldToRemove)
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

/**
 * @brief Print the user's input, fields that the input is a substring of,
 * and fields that the user has already picked. Prints via ncurses.
 * 
 * @param[in] stdscr - used for ncurses output
 * @param[in] userInput - Printed out along with matched/picked fields
 */
void CLI::printFields(WINDOW* stdscr, const std::string& userInput) const
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
    addstr("---------\n");
    addstr((">" + userInput + "\n").c_str());
}

/**
 * @brief Prompts the user for input and tells the user what fields
 * are available to be picked.
 * 
 * @return FieldMap - The fields the user has picked
 */
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
                handleBackspace(userInput);
                break;
            case KEY_ENTER:
            case 36:
            case 10:
            /* User has pressed enter ---------- */
                if(exitOnEnter(userInput))
                    return fieldsToFieldMap(m_pickedFields);
                break;
            default:
            /* User has pressed any key other
               than ENTER or BACKSPACE ---------- */
                handleNewChar(userInput, static_cast<char>(ch));
                break;
        } // End of switch(ch)

        clear();
        // Print every cycle
        printFields(stdscr, userInput);

    } // End of for (;;)

}

/**
 * @brief Handles a backspace character sent from the user
 * 
 * @param[out] userInput - Removes a character from the user's input
 */
void CLI::handleBackspace(std::string& userInput)
{
    if (userInput.size() > 0)
        userInput.pop_back();
    m_matchedFields = fuzzySearchFields(userInput);
}

/**
 * @brief Handles an enter character sent from the user. Depending
 * on context, the user may want to stop picking fields.
 * 
 * @param[out] userInput - Clears the string
 * @return true - The user is done picking values
 * @return false - The user is not done picking values
 */
bool CLI::exitOnEnter(std::string& userInput)
{
    bool timeToExit = false;
    if (userInput.empty())
    {   // User is done making choices
        clear();
        endwin();
        timeToExit = true;
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

    return timeToExit;
}

/**
 * @brief Appends the new character to the user's input string
 * 
 * @param[out] userInput - Appends to string
 * @param[in] newChar - The new character to add
 */
void CLI::handleNewChar(std::string& userInput, char newChar)
{
    userInput += newChar;
    m_matchedFields = fuzzySearchFields(userInput);
}

/**
 * @brief Converts a vector of fields (which is just the names) to a FieldMap
 * which includes the values of the fields as well
 * 
 * @param[in] fields - Vector of field names
 * @return FieldMap - Field names and values
 */
FieldMap CLI::fieldsToFieldMap(const std::vector<std::string_view>& fields)
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