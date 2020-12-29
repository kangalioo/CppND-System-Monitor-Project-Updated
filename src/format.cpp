#include "format.h"

#include <cstdio>
#include <sstream>
#include <string>
#include <iomanip>

using std::string;

string Format::ElapsedTime(long seconds) {
	auto minutes = seconds / 60;
	seconds = seconds % 60;

	auto hours = minutes / 60;
	minutes = minutes % 60;

	std::stringstream output;
	output << hours << ":" <<
		std::setw(2) << std::setfill('0') << minutes << ":" <<
		std::setw(2) << std::setfill('0') << seconds;
	return output.str();
}