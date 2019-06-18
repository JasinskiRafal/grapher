#include "cli.hpp"
#include "csv_logdb.hpp"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: grapher <file.csv>" << std::endl;
        return 0;
    }
    // Take the file as input
    std::string file(argv[1]);
    LogDatabase logDb = LogDatabase(file);
    GrapherCLI cli;
    // Get the fields from our database
    std::vector<std::string> allFields = logDb.getFields();
    // Figure out which fields the user wants to visualize
    std::vector<std::string> pickedFields = cli.getFieldsFromUser(allFields);

    return 0;
}