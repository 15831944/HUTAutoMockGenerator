#pragma once
#include <iostream>
#include <sstream>

class CommonData
{
public:
	CommonData(std::ostream& st, const std::string& orgFN, const std::string& clasName) :
		currentIdent(0),
		stream(st),
		originalFileName(orgFN),
		currentFileName(orgFN),
		destinationFileName(""),
		classNameToMock(clasName)
	{		
	}

	unsigned char currentIdent;
	std::ostream& stream;
	std::string originalFileName;
	std::string currentFileName;
	std::string destinationFileName;
	std::string classNameToMock;
	
	bool doAllClasses() const
	{
		return classNameToMock.empty();
	}
};
