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
    allFields = logDb.getFields();
    
    // Figure out which fields the user wants to visualize
    GrapherCLI cli;
    pickedFields = cli.getFieldsFromUser(allFields);

    return 0;
}