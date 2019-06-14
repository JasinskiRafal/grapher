#include "csv_row.hpp"

/////////////////////////////////////////////
// CSVRow implemenation
/////////////////////////////////////////////

std::size_t CSVRow::size() const
{
	return m_data.size();
}

void CSVRow::readNextRow(std::istream& str)
{
	std::string       line;
	std::getline(str, line);

	std::stringstream lineStream(line);
	std::string       cell;

	m_data.clear();
	while(std::getline(lineStream, cell, ','))
	{
			m_data.push_back(cell);
	}
	// Check for comma with no data after it
	if (!lineStream && cell.empty())
	{
			m_data.push_back(""); // Add an empty element
	}
}

std::string const& CSVRow::operator[](std::size_t index) const
{
	return m_data[index];
}
