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

std::string GetOnlyFileName(const std::string& fileName)
{
	auto pos1 = fileName.find_last_of('\\');
	auto pos2 = fileName.find_last_of('.');
	std::string onlyFileName = "";
	if (pos1 != std::string::npos && pos2 != std::string::npos)
	{
		onlyFileName = fileName.substr(pos1 + 1, pos2 - pos1 - 1);
	}
	else if (pos1 != std::string::npos)
	{
		onlyFileName = fileName.substr(pos1 + 1);
	}
	else if (pos2 != std::string::npos)
	{
		onlyFileName = fileName.substr(0, pos2);
	}
	else
	{
		onlyFileName = fileName;
	}
	return onlyFileName;
}
