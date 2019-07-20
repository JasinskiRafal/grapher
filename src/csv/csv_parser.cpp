#include "csv_parser.hpp"

/////////////////////////////////////////////
// CSVRow implemenation
/////////////////////////////////////////////

/**
 * @brief Index into a csv row
 * 
 * @param[in] index 
 * @return std::string const& 
 */
std::string const& CSVRow::operator[](std::size_t index) const
{
    return m_data[index];
}

/**
 * @brief Get size of the data in a CSVRow
 * 
 * @return std::size_t 
 */
std::size_t CSVRow::size() const
{
    return m_data.size();
}

/**
 * @brief Read the next row from the input stream
 * 
 * @param[in] str 
 */
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
/////////////////////////////////////////////
// Used in both CSVRow and CSVIterator 
/////////////////////////////////////////////
/**
 * @brief Used in CSVIterator to stream the input stream to
 * a CSVRow
 * 
 * @param[in] str - the input stream
 * @param[out] data - the CSVRow given new data
 */
std::istream& operator>>(std::istream& str, CSVRow& data)
{
	data.readNextRow(str);
	return str;
}


/////////////////////////////////////////////
// CSVIterator implemenation
/////////////////////////////////////////////
/**
 * @brief Constructs an iterator from an input stream. Sets m_str to
 * &str if the stream is good, otherwise it sets m_str to null.
 * 
 * @param[in] str - The input stream
 */
CSVIterator::CSVIterator(std::istream& str) :m_str(str.good()? &str : NULL)
{
    ++(*this);
}

/**
 * @brief Default constructor 
 */
CSVIterator::CSVIterator() :m_str(NULL) {}

/**
 * @brief iterates to the next row in the csv
 * 
 * @return CSVIterator& 
 */
CSVIterator& CSVIterator::operator++()
{
    if (m_str)
    {
            if (!((*m_str) >> m_row))
            {
                    m_str = NULL;
            }
    }
    return *this;
}

/**
 * @brief Increments the iterator
 * 
 * @return CSVIterator 
 */
CSVIterator CSVIterator::operator++(int)
{
    CSVIterator tmp(*this);
    ++(*this);
    return tmp;
}

/**
 * @brief Dereferences the iterator to get to its data (the row)
 * 
 * @return CSVRow const& 
 */
CSVRow const& CSVIterator::operator*() const { return m_row; }

/**
 * @brief Gets the address of the iterators current row (m_row)
 * 
 * @return CSVRow const* 
 */
CSVRow const* CSVIterator::operator->() const { return &m_row; }

/**
 * @brief Checks if two iterators are pointing to the same thing
 * or if they're both null.
 * 
 * @param[in] rhs - The other iterator
 * @return true - Both iterators are pointing to the same thing or
 * both are null.
 * @return false - Iterators have different values
 */
bool CSVIterator::operator==(CSVIterator const& rhs)
{
    return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));
}

/**
 * @brief Checks if the value of this iterator is equal to the other iterator.
 * 
 * @param[in] rhs - Other iterator
 * @return true - Values are not equal
 * @return false - Values are equal
 */
bool CSVIterator::operator!=(CSVIterator const& rhs)
{
    return !((*this) == rhs);
}
