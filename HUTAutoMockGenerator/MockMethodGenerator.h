#pragma once
#include <iostream>
#include <sstream>
#include "MockClassGenerator.h"

class MockMethodGenerator
{
public:
	void startMockMethod(std::ostream& stream, std::string name, int num_args, std::string return_type, bool isConst, int level = 0);
	void addParameter(std::ostream& stream, std::string type, std::string name, bool isLast = false);
	void endMockMethod(std::ostream& stream);

};

