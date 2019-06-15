#include <iostream>
#include <memory>

#include "csv_logdb.hpp"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cout << "Usage csv_driver <file.csv>" << std::endl;
		return 0;
	}
	// Take the file as input
	LogDatabase logDb;
	std::cout << "Parsing file:" << argv[1] << std::endl;
	logDb.parseFile(std::string(argv[1]));
	std::vector<std::string> v = logDb.getFields();
	for (auto const& f : v)
	{
		std::cout << f << std::endl;
	}

	// Just grab whatever's at the front
	std::string field = v.front();
	std::vector<std::string> values = logDb.getFieldValues(field);
	std::cout << "Num of values in field:" << values.size() << std::endl;


	return 0;
}