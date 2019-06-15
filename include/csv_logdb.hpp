#ifndef CSV_LOGDB_HPP
#define CSV_LOGDB_HPP

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <variant>

namespace logdb
{
	using flexVector =
		std::variant<std::vector<int>, std::vector<float>, std::vector<std::string>>;

	class LogVariable
	{
		public:
			LogVariable(std::string fieldName);
			~LogVariable();

			void addData(std::string value); // Deduce the data type and add it to vector

		private:
			std::string fieldName;
			// A vector of ints, a vector of floats, or a vector of strings:
			flexVector values;

	};

	class LogDatabase
	{
		public:
			LogDatabase(std::string filename);
			LogDatabase();
			~LogDatabase();
			void parseFile(std::string filename);
			// Search for the field name in the database, grab the corresponding data points
			flexVector* getDataPoints(std::string fieldName);
		private:
			std::vector<LogVariable> logVars;
	};
}
#endif
