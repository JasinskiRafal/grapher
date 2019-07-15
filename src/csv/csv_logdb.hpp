#ifndef CSV_LOGDB_HPP
#define CSV_LOGDB_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>

// This is going to be the standard data type for storing fields and their values
using FieldMap = std::unordered_map<std::string, std::vector<std::string>>;

// Declared outside of class for general use with unordered maps
template <class T, class R>
std::vector<T> getKeys(std::unordered_map<T, R> map)
{
    // No built in method to grab keys from unordered_map
    std::vector<T> v;
    // Go through each pair in the map, grab the key
    for(auto const& pair: map)
    {
        v.push_back(pair.first);
    }
    return v;
}


class LogDatabase
{
    public:
        LogDatabase(std::string filename);
        LogDatabase();
        void parseFile(std::string filename);
        std::vector<std::string> getValuesOfField(std::string fieldname) const;
        const std::vector<std::string> getFieldnames() const;
        FieldMap getFieldMap() const;
    private:
        FieldMap m_logMap;
};
#endif
