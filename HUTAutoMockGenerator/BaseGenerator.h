#pragma once
#include <iostream>
#include <sstream>
#include "Utils.h"

class BaseGenerator
{
public:
	virtual void addEmptyLine(std::ostream& stream) { stream << std::endl; }
	virtual void AddInclude(std::ostream& stream, const std::string& file, int level = 0);
	virtual void addParameter(std::ostream& stream, std::string type, std::string name, bool isLast = false);
};

