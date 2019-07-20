#include <thread> 

#include "cli.hpp"
#include "csv_logdb.hpp"

int main(int argc, char** argv)
{
    FieldMap pickedFields;

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
    CLI cli(logDb);
    Window w;

    // Grab the first round of fields
    pickedFields = cli.getFieldsFromUser();
    w.replaceFields(pickedFields);

    for (;;)
    {
        std::thread output(drawWindow, w);
        // Get input for next round
        pickedFields = cli.getFieldsFromUser();
        // End the output window thread since we must redraw
        output.join();
        w.replaceFields(pickedFields);
    }


    return 0;
}