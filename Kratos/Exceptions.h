#pragma once
#include <exception>
#include <string>
#include <sstream>

class DetailedException : public std::exception
{
public:
	DetailedException(const char* reason, std::string func, std::string file, int line) : std::exception(reason)
	{
		std::ostringstream stringStream;
		stringStream << func << " " << file << " line " << line;
		Place = stringStream.str();
	}
	std::string Place;
};

#define EXCEPTION(reason) DetailedException(reason, __FUNCSIG__, __FILE__, __LINE__)