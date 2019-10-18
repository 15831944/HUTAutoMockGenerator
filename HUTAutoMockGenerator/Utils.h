#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <clang-c/Index.h>

std::ostream& operator<<(std::ostream& stream, const CXString& str);
std::string ToString(const CXString& s);
std::string tabIndent(int level);
std::string GetFileNameFromPath(const std::string& f);
