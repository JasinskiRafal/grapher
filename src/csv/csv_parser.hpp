#ifndef CSV_PARSER_HPP
#define CSV_PARSER_HPP

#include <iterator>
#include <iterator>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

// Grabbed from:
// https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c?page=1&tab=votes#tab-top

/////////////////////////////////////////////
// CSVRow class
/////////////////////////////////////////////
/**
 * @brief A single row in a CSV file. Given an inputstream, it can read to the next row
 */
class CSVRow
{
    public:
        std::string const& operator[](std::size_t index) const;
        std::size_t size() const;
        void readNextRow(std::istream& str);

    private:
        std::vector<std::string>    m_data;
};

// Used in both CSVRow and CSVIterator
std::istream& operator>>(std::istream& str, CSVRow& data);

/////////////////////////////////////////////
// CSVIterator class
/////////////////////////////////////////////
/**
 * @brief A class for iterating through an entire CSV file.
 */
class CSVIterator
{
    public:
        /** @brief Allows the class to be used as an iterator */
        typedef std::input_iterator_tag     iterator_category;
        /** @brief The value we are iterating over is CSV rows */
        typedef CSVRow                      value_type;
        /** @brief Needed for satisfying iterator traits */
        typedef std::size_t                 difference_type;
        /** @brief Needed for satisfying iterator traits */
        typedef CSVRow*                     pointer;
        /** @brief Needed for satisfying iterator traits */
        typedef CSVRow&                     reference;

        CSVIterator(std::istream& str);
        CSVIterator();

        CSVIterator& operator++();
        CSVIterator operator++(int);
        CSVRow const& operator*() const;
        CSVRow const* operator->() const;
        bool operator==(CSVIterator const& rhs);
        bool operator!=(CSVIterator const& rhs);

    private:
        std::istream*   m_str;
        CSVRow          m_row;
};

#endif