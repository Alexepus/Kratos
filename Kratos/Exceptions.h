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

	// Если ex является DetailedException, то возвращает место ошибки
	static inline std::string TryGetExceptionPlace(const std::exception& ex)
	{
		auto detailedException = dynamic_cast<const DetailedException*>(&ex);
		if (detailedException != nullptr)
			return std::string(" (в ") + detailedException->Place + ")";
		return "";
	}

	static inline std::string TryGetInnerException(const std::exception& ex)
	{
		auto detailedException = dynamic_cast<const DetailedException*>(&ex);
		if (detailedException != nullptr && detailedException->InnerExceptionInfo.length() > 0)
			return std::string("\nВнутреннее исключение: " + detailedException->InnerExceptionInfo);
		return "";
	}

	static inline std::string GetExceptionType(const std::exception& ex)
	{
		auto typeDescr = std::string(typeid(ex).name());
		auto pos = typeDescr.find("class ");
		if (pos != std::string::npos)
			return typeDescr.substr(pos + 6);
		return typeDescr;
	}

	// Если ex является DetailedException, то возвращает причину и место ошибки
	static inline std::string TryGetDetailedWhat(const std::exception& ex)
	{
		return GetExceptionType(ex) + ": " +  ex.what() + TryGetExceptionPlace(ex) + TryGetInnerException(ex);
	}
};

class CounterTimeoutException : public DetailedException
{
public:
	CounterTimeoutException(const char* reason, std::string func, std::string file, int line)
		: DetailedException(reason, func, file, line)
	{}
};

class FileOpenException : public DetailedException
{
public:
	FileOpenException(const char* reason, std::string func, std::string file, int line)
		: DetailedException(reason, func, file, line)
	{}
};


#define EXCEPTION(reason) DetailedException(reason, __FUNCSIG__, __FILE__, __LINE__)
#define EXCEPTION_WITH_INNER(reason, innerException) DetailedException(reason, innerException, __FUNCSIG__, __FILE__, __LINE__)
#define EXCEPTION_SPECIFIC(exceptionType, reason) exceptionType(reason, __FUNCSIG__, __FILE__, __LINE__)
#define EXCEPTION_SPECIFIC_WITH_INNER(exceptionType, reason, innerException) exceptionType(reason, innerException, __FUNCSIG__, __FILE__, __LINE__)
