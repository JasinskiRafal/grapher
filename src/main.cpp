#include <thread>
#include <chrono>

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
    
    std::thread outputWindow(drawWindow, Window(pickedFields, logDb));

    outputWindow.join();

    return 0;
}