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

	DetailedException(const char* reason, const std::exception& innerException, std::string func, std::string file, int line) : DetailedException(reason, func, file, line)
	{
		InnerExceptionInfo = TryGetDetailedWhat(innerException);
	}

	std::string Place;
	std::string InnerExceptionInfo;

	// ���� ex �������� DetailedException, �� ���������� ����� ������
	static inline std::string TryGetExceptionPlace(const std::exception& ex)
	{
		auto detailedException = dynamic_cast<const DetailedException*>(&ex);
		if (detailedException != nullptr)
			return std::string(" (� ") + detailedException->Place + ")";
		return "";
	}

	static inline std::string TryGetInnerException(const std::exception& ex)
	{
		auto detailedException = dynamic_cast<const DetailedException*>(&ex);
		if (detailedException != nullptr && detailedException->InnerExceptionInfo.length() > 0)
			return std::string("\n���������� ����������: " + detailedException->InnerExceptionInfo);
		return "";
	}

	// ���� ex �������� DetailedException, �� ���������� ������� � ����� ������
	static inline std::string TryGetDetailedWhat(const std::exception& ex)
	{
		return std::string(ex.what()) + TryGetExceptionPlace(ex) + TryGetInnerException(ex);
	}
};

#define EXCEPTION(reason) DetailedException(reason, __FUNCSIG__, __FILE__, __LINE__)
#define EXCEPTION_WITH_INNER(reason, innerException) DetailedException(reason, innerException, __FUNCSIG__, __FILE__, __LINE__)