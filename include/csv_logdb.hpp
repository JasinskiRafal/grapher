#ifndef CSV_LOGDB_HPP
#define CSV_LOGDB_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <unordered_map>

// Declared outside of class for general use with unordered maps
template <class T, class R>
std::vector<T> getKeys(std::unordered_map<const T, R> map)
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
        std::vector<std::string_view> getValuesOfField(std::string fieldname);
        std::vector<std::string_view> getFieldnames();
    private:
        std::unordered_map<
            const std::string, std::vector<const std::string>> m_logMap;
};
#endif
