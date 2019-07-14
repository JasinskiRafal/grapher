#include <algorithm> 
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

void LogDatabase::parseFile(std::string filename)
{
    std::ifstream             file(filename);
    CSVIterator               row(file);
    std::vector<std::string>  fields;

    // Grab fieldnames from first row
    for (int i = 0; i < (*row).size() - 1; i++)
    {
        fields.push_back((*row)[i]);
        // This field maps to a vector of strings, initialize the
        // vector of strings.
        // If we access them without doing this, we get a segfault
        m_logMap[fields[i]].clear();
    }

    auto nFields = m_logMap.size();
    row++; // Move to next row

    // Now grab the values for each field 
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
std::vector<std::string> LogDatabase::getValuesOfField(std::string fieldname)
{
    auto search = m_logMap.find(fieldname);
    if (search == m_logMap.end())
    {
        // Could not find the field
        return {};
    }
    else
    {
        // Return the vector containing the data points
        return search->second;
    }
    
}

std::vector<std::string> LogDatabase::getFieldnames()
{
    // Get all the keys out of the unordered_map
    return getKeys(m_logMap);
}

fieldMap LogDatabase::getFieldMap()
{
    return m_logMap;
}