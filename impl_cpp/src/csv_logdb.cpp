#include "csv_iterator.hpp"
#include "csv_logdb.hpp"

using namespace logdb;

/////////////////////////////////////////////
// LogVariable implemenation
/////////////////////////////////////////////
LogVariable::LogVariable(std::string fieldName)
{
	this->fieldName = fieldName;
}
LogVariable::~LogVariable() {};

void LogVariable::addData(std::string value)
{
	// TODO add a value to the variable's history of values
}

/////////////////////////////////////////////
// LogDatabase implemenation
/////////////////////////////////////////////
LogDatabase::LogDatabase(std::string filename)
{
	parseFile(filename);
}
LogDatabase::LogDatabase() {};
LogDatabase::~LogDatabase() {};

void LogDatabase::parseFile(std::string filename)
{
	std::ifstream   file(filename);
	// TODO implement

	for(CSVIterator loop(file); loop != CSVIterator(); ++loop)
	{
		// Load data into class structure here
		std::cout << "0th element of *loop:" << (*loop)[0]  << std::endl;

		// For each field, create a new LogVariable
		// After fields are loaded, add data points to each variable
	}

	return;
}

logdb::flexVector* LogDatabase::getDataPoints(std::string fieldName)
{
	// Search for the field name in the database, grab the corresponding data points
	// TODO Implement
	logdb::flexVector* fvPtr = new logdb::flexVector();
	return fvPtr;
}
