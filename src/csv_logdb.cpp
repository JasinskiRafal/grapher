#include "csv_parser.hpp"
#include "csv_logdb.hpp"

using namespace logdb;

/////////////////////////////////////////////
// LogVariable implemenation
/////////////////////////////////////////////
LogVariable::LogVariable()
{
    this->m_type = UNDEFINED;
}
LogVariable::~LogVariable() {};

logdb::varType LogVariable::deduceType(std::string value)
{
    std::size_t charsRead;
    try {
        int decimalValue = std::stoi(value, &charsRead);
    }
    catch (std::invalid_argument& e){
        charsRead = 0;
    }

    try {
        float floatValue = std::stof(value, &charsRead);
    }
    catch (std::invalid_argument& e){
        charsRead = 0;
    }

    if (charsRead == value.size())
    {
        // Converted whole number to a int, type is int
        // This means we have a vector of ints
        m_values.emplace<std::vector<int>>();
        return INT;
    }
    else if (charsRead == value.size())
    {
        // Converted whole number to float, type is float
        // This means we have a vector of floats
        m_values.emplace<std::vector<float>>();
        return FLOAT;
    }
    else
    {
        // Otherwise, make it a string
        // This means we have a vector of strings 
        m_values.emplace<std::vector<std::string>>();
        return STRING;
    }
}

logdb::varType LogVariable::getType()
{
    return m_type;
}

void LogVariable::addData(std::string value)
{
    // If we dont know the type yet, figure it out
    if (m_type == UNDEFINED)
        this->m_type = deduceType(value);


    if (m_type == INT)
    {
        int intVal = std::stoi(value);
        // Get the vector out of the variant and add onto it
        (std::get<std::vector<int>>(this->m_values))
            .push_back(intVal);

    }
    else if(m_type == FLOAT)
    {
        float floatVal = std::stoi(value);
        // Get the vector out of the variant and add onto it
        (std::get<std::vector<float>>(this->m_values))
            .push_back(floatVal);
    }
    else if(m_type == STRING)
    {
        // Get the vector out of the variant and add onto it
        (std::get<std::vector<std::string>>(this->m_values))
            .push_back(value);
    }
    else
    {
        std::cerr << "LogVariable::addData Invalid variable type: "
            << m_type << ", value: " << value << std::endl;
    }
}

logdb::variantVector LogVariable::getValues()
{
    return this->m_values;
}

/////////////////////////////////////////////
// LogDatabase implemenation
/////////////////////////////////////////////
LogDatabase::LogDatabase(std::string filename)
{
    parseFile(filename);
}
LogDatabase::LogDatabase() {};
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
        // Our field maps to a LogVariable 
        m_logMap[fields.at(i)] = new logdb::LogVariable();
    }

    auto nFields = m_logMap.size();
    row++; // Move to next row

    // Now grab data points for each field 
    while(row != CSVIterator())
    {
        for(int i = 0; i < nFields; i++)
        {
            // Add the data from this row to the corresponding fieled
            (m_logMap[fields.at(i)])->addData((*row)[i]);
        }
        row++;
    }
    return;
}

// Search for the field name in the database, grab the corresponding data points
LogVariable LogDatabase::getDataPoints(std::string fieldname)
{
    auto search = m_logMap.find(fieldname);
    if (search == m_logMap.end())
    {
        // Could not find the field
        return {};
    }
    else
    {
        // return the variant vector containing the data points 
        return *(search->second);
    }
    
}

std::vector<std::string> LogDatabase::getFieldList()
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