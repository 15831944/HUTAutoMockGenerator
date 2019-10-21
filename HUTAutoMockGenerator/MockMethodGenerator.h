#pragma once
#include <iostream>
#include <sstream>
#include "MockClassGenerator.h"

class MockMethodGenerator : public BaseGenerator
{
public:
	void startMockMethod(std::ostream& stream, std::string name, int num_args, std::string return_type, bool isConst, int level = 0);
	void endMockMethod(std::ostream& stream);
};

