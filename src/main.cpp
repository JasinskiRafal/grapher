#include "cli.hpp"
#include "csv_logdb.hpp"

int main(int argc, char** argv)
{
    std::vector<std::string> pickedFields;

    if (argc != 2)
    {
        std::cout << "Usage: grapher <file.csv>" << std::endl;
        return 1;
    }

    // Take the file as input
    std::string file(argv[1]);
   
    // Parse the CSV
    LogDatabase logDb = LogDatabase(file);
    // Get the fields from our database
    
    CLI cli;
    // Figure out which fields the user wants to visualize
    pickedFields = cli.getFieldsFromUser(logDb.getFieldnames());

    // Graph the chosen values from the database 
    GraphGroup gG(pickedFields, logDb);
    // Add the group to a window
    Window w(gG);

    w.draw();

    return 0;
}