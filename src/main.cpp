#include "cli.hpp"
#include "csv_logdb.hpp"

int main(int argc, char** argv)
{
    std::vector<std::string> pickedFields;
    std::vector<std::string> allFields;

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
    allFields = logDb.getFieldnames();
    
    // Figure out which fields the user wants to visualize
    CLI cli;
    pickedFields = cli.getFieldsFromUser(allFields);

    // Just grab the field off the front for testing
    std::string fieldname = pickedFields.front();

    auto field = std::make_pair (fieldname, logDb.getValuesOfField(fieldname));
    // Graph the field
    Graph g(field);
    // Make the graph into a group
    GraphGroup gG(g);
    // Add the group to a window
    Window w(gG);

    w.draw();

    return 0;
}