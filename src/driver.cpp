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
	logdb::LogDatabase logDb;
	// Take the file as input
	std::cout << "Parsing file:" << argv[1] << std::endl;
	logDb.parseFile(std::string(argv[1]));
	std::vector v = logDb.getFieldList();
	for (auto const& f : v)
	{
		std::cout << f << std::endl;
	}
	std::string field = v.front();
	logdb::LogVariable lV = logDb.getDataPoints(field);
	if (lV.getType() == logdb::INT)
	{
		std::vector<int> v = std::get<std::vector<int>>(lV.getValues());
		std::cout << "Num of values in LogVariable:" << v.size() << std::endl;
	}
	else if(lV.getType() == logdb::FLOAT)
	{
		std::vector<float> v = std::get<std::vector<float>>(lV.getValues());
		std::cout << "Num of values in LogVariable:" << v.size() << std::endl;
	}
	else if(lV.getType() == logdb::STRING)
	{
		std::vector<std::string> v = std::get<std::vector<std::string>>(lV.getValues());
		std::cout << "Num of values in LogVariable:" << v.size() << std::endl;
	}


	return 0;
}