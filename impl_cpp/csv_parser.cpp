#include <ifstream>
#include <iostream>
#include <sfstream>
#include <string>
#include <vector>
#include <iterator>

class LogVariable
{
    public:
        LogVariable(std::string fieldName)
        {
            this.fieldName = fieldName;
        }

        void addDataPoint(T genericValue)
        {
            // Deduce the value type, instantiate value vector once this is known
            // It is probably going to be taken in as a string but must be interpreted
            
            return;
        }
    private:
        std::string fieldName;
        vector<T> values;


}

class LogDatabase
{
    public:
        LogDatabase(std::string filename)
        {
            this.parseFile(filename);
        }

        void parseFile(std::string filename)
        {
            std::ifstream   file(filename);

            for(CSVIterator loop(file); loop != CSVIterator(); ++loop)
            {
                // Load data into class structure here

                // For each field, create a new LogVariable

                // After fields are loaded, add data points to each variable
            }

            return;
        }

        vector<T> getDataPoints(std::string fieldName) const
        {
            // Search for the field name in the database, grab the corresponding data points
        }

    private:
        vector<LogVariable> logVars;
}


class CSVRow
{
    // Grabbed from:
    // https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c?page=1&tab=votes#tab-top
    public:
        std::string const& operator[](std::size_t index) const
        {
            return m_data[index]
        }

        std::size_t size() const
        {
            return m_data.size();
        }

        void readNextRow(std::istream& str)
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
                // Add an empty element
                m_data.push_back("");
            }

            return;
        }

    private:
        std::vector<std::string>    m_data;
}

class CSVIterator
{
    public:
        typedef std::input_iterator::tag    iterator_category;
        typedef CSVRow                      value_type;
        typedef std::size_t                 difference_type;
        typedef CSVRow*                     pointer;
        typedef CSVRow&                     reference;

        CSVIterator(std::istream& str)      :m_str(str.good()?&str:NULL)
        {
            ++(*this);
        }
        CSVIterator()                       :m_str(NULL)
        {

        }

        CSVIterator& operator++()
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

        CSVIterator operator++(int)
        {
            CSVIterator tmp(*this);
            ++(*this);
            return tmp;
        }

        CSVRow const& operator*() const
        {
            return m_row;
        }

        CSVRow const* operator->() const
        {
            return &m_row;
        }

        bool operator ==(CSVIterator const& rhs)
        {
            return ((this == &rhs) || ((this->m_str == NULL) && (rhs.m_str == NULL)));
        }

        bool operator ==(CSVIterator const& rhs)
        {
            return !((*this) == rhs);
        }

    private:
        std::istream*   m_str;
        CSVRow          m_row;
}
