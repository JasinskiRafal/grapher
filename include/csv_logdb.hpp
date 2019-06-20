#ifndef CSV_LOGDB_HPP
#define CSV_LOGDB_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <utility>
#include <unordered_map>

class LogDatabase
{
    public:
        LogDatabase(std::string filename);
        LogDatabase();
        ~LogDatabase();
        void parseFile(std::string filename);
        std::vector<std::string> getFieldValues(std::string fieldname);
        std::vector<std::string> getFields();
        std::unordered_map<std::string, std::vector<std::string>> getFieldMap();
    private:
        std::unordered_map<std::string, std::vector<std::string>> m_logMap;
};
#endif
