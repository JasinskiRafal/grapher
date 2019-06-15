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
class CSVIterator
{
	public:
		typedef std::input_iterator_tag     iterator_category;
		typedef CSVRow                      value_type;
		typedef std::size_t                 difference_type;
		typedef CSVRow*                     pointer;
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