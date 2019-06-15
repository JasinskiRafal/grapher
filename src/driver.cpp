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
	logDb.parseFile(std::string(argv[1]));

	return 0;
}
