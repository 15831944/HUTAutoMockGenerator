#include "Utils.h"


std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
	clang_disposeString(str);
	return stream;
}

std::string ToString(const CXString& s)
{
	std::string result = clang_getCString(s);
	clang_disposeString(s);
	return result;
}

std::string tabIndent(int level)
{
	return std::string(level * 4, ' ');
}

std::string GetFileNameFromPath(const std::string& f)
{
	if (f.find_last_of("\\") != std::string::npos)
	{
		return f.substr(f.find_last_of("\\") + 1);
	}
	return f;
}