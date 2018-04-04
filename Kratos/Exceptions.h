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

	// ���� ex �������� DetailedException, �� ���������� ����� ������
	static inline std::string TryGetExceptionPlace(std::exception ex)
	{
		auto detailedException = dynamic_cast<DetailedException*>(&ex);
		if (detailedException != nullptr)
			return std::string(" (� ") + detailedException->Place + ")";
		return "";
	}

	// ���� ex �������� DetailedException, �� ���������� ������� � ����� ������
	static inline std::string TryGetDetailedWhat(std::exception ex)
	{
		return std::string(ex.what()) + TryGetExceptionPlace(ex);
	}
};



#define EXCEPTION(reason) DetailedException(reason, __FUNCSIG__, __FILE__, __LINE__)