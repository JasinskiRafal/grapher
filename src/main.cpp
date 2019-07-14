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
    
    // Figure out which fields the user wants to visualize
    CLI cli;
    pickedFields = cli.getFieldsFromUser(logDb.getFieldnames());

    Window w(pickedFields, logDb);
    // Make a thread of outputWindow()
    std::thread outputWindow(drawWindow, w);

    outputWindow.join();

    return 0;
}