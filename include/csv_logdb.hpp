#ifndef CSV_LOGDB_HPP
#define CSV_LOGDB_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <variant>
#include <unordered_map>

namespace logdb
{
    using variantVector =
        std::variant<std::vector<int>,std::vector<float>,std::vector<std::string>>;
    // Supported types
    enum varType {UNDEFINED, INT, FLOAT, STRING};

    class LogVariable
    {
        // This describes a variable. Supplies the type as well as the
        // history of data points in m_values
        public:
            LogVariable();
            ~LogVariable();
            void addData(std::string value);
            variantVector getValues();
            varType getType();

        private:
            varType deduceType(std::string value);
            variantVector m_values;
            varType  m_type;

    };

    class LogDatabase
    {
        public:
            LogDatabase(std::string filename);
            LogDatabase();
            ~LogDatabase();
            void parseFile(std::string filename);
            LogVariable getDataPoints(std::string fieldname);
            std::vector<std::string> getFieldList();
        private:
            std::unordered_map<std::string, LogVariable*> m_logMap;
    };
}
#endif
