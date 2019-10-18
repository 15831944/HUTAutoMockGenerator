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
