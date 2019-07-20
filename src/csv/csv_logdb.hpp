#ifndef CSV_LOGDB_HPP
#define CSV_LOGDB_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>
#include <utility>

// This is going to be the standard data type for storing fields and their values
using FieldMap  = std::unordered_map<std::string, std::vector<std::string>>;
using FieldPair = std::pair<std::string, std::vector<std::string>>;

// Declared outside of class for general use with unordered maps
/**
 * @brief Retrieve all keys from an unordered map
 * 
 * @tparam[in] T - generic class
 * @tparam[in] R - generic class
 * @param[in] map - The map to search through
 * @return std::vector<T> - The keys
 */
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

/**
 * @brief Object for storing all of the .csv values in a single place
 */
class LogDatabase
{
    public:
        LogDatabase(const std::string& filename);
        LogDatabase();
        void parseFile(const std::string& filename);
        std::vector<std::string> getValuesOfField(const std::string& fieldname) const;
        std::vector<std::string> getFieldnames() const;
        FieldMap getFieldMap() const;
    private:
        FieldMap m_logMap;
};
#endif
