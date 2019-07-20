#include <algorithm> 
#include "csv_parser.hpp"
#include "csv_logdb.hpp"

/////////////////////////////////////////////
// LogDatabase implemenation
/////////////////////////////////////////////
/**
 * @brief Reads in filename and creates a log database out of it
 * 
 * @param[in] filename - name of the .csv file
 */
LogDatabase::LogDatabase(const std::string& filename)
{
    parseFile(filename);
}

/**
 * @brief Default constructor 
 */
LogDatabase::LogDatabase()
{
}

/**
 * @brief Reads through a csv file and populates LogDatabase's m_logMap
 * with the values from filename
 * 
 * @param[in] filename - Input filename
 */
void LogDatabase::parseFile(const std::string& filename)
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

/**
 * @brief Search for the field name in the database, grab the corresponding data points
 * 
 * @param[in] fieldname - field to search for
 * @return std::vector<std::string> - values of the field
 */
std::vector<std::string>
LogDatabase::getValuesOfField(const std::string& fieldname) const
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

/**
 * @brief Get all of the fields in the LogDatabase
 * 
 * @return std::vector<std::string> - vector of fieldnames
 */
std::vector<std::string> LogDatabase::getFieldnames() const
{
    // Get all the keys out of the unordered_map
    return getKeys(m_logMap);
}

/**
 * @brief Returns a copy of the LogDatabase's fieldmap
 * 
 * @return FieldMap 
 */
FieldMap LogDatabase::getFieldMap() const
{
    return m_logMap;
}