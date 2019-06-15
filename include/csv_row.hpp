#ifndef CSV_ROW_HPP
#define CSV_ROW_HPP

#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

/////////////////////////////////////////////
// CSVRow Header 
/////////////////////////////////////////////

class CSVRow
{
	// Grabbed from:
	// https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c?page=1&tab=votes#tab-top
	public:
		std::string const& operator[](std::size_t index) const;
		std::size_t size() const;
		void readNextRow(std::istream& str);

	private:
		std::vector<std::string>    m_data;
};

inline std::istream& operator>>(std::istream& str, CSVRow& data)
{
	data.readNextRow(str);
	return str;
}

#endif
