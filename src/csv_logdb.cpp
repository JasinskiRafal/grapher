#include "csv_parser.hpp"
#include "csv_logdb.hpp"

/////////////////////////////////////////////
// LogDatabase implemenation
/////////////////////////////////////////////
LogDatabase::LogDatabase(std::string filename)
{
    parseFile(filename);
}
LogDatabase::LogDatabase()
{
}
LogDatabase::~LogDatabase() {};

void LogDatabase::parseFile(std::string filename)
{
    std::ifstream             file(filename);
    CSVIterator               row(file);
    std::vector<std::string>  fields;

    // Grab fieldnames from first row
    for (int i = 0; i < (*row).size() - 1; i++)
    {
        fields.push_back((*row)[i]);
        // Our field maps to a vector of strings, initialize them
        // If we access them without doing this, we get a segfault
        m_logMap[fields[i]].clear();
    }

    auto nFields = m_logMap.size();
    row++; // Move to next row

    // Now grab data points for each field 
    while(row != CSVIterator())
    {
        for(int i = 0; i < nFields; i++)
        {
            // Add the data from this row to the corresponding field
            (m_logMap[fields[i]]).push_back((*row)[i]);
        }
        row++;
    }
    return;
}

// Search for the field name in the database, grab the corresponding data points
std::vector<std::string> LogDatabase::getFieldValues(std::string fieldname)
{
    auto search = m_logMap.find(fieldname);
    if (search == m_logMap.end())
    {
        // Could not find the field
        return {};
    }
    else
    {
        // return the vector containing the data points 
        return search->second;
    }
    
}

std::vector<std::string> LogDatabase::getFields()
{
    // No build in method to grab keys from unordered_map
    std::vector<std::string> v;
    // Go through each pair in the map, grab the key
    for(auto const& pair: m_logMap)
    {
        v.push_back(pair.first);
    }
    return v;
}