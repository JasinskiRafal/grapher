#ifndef CSV_ITERATOR_HPP
#define CSV_ITERATOR_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iterator>

#include "csv_row.hpp"

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
