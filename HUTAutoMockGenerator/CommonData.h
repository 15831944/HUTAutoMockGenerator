#pragma once
#include <iostream>
#include <sstream>
#include "ClassSourceGenerator.h"

class CommonData
{
public:
	CommonData(std::ostream& st, 
		const std::string& orgFN, 
		const std::string& clasName, 
		const std::string& destFile,
		const std::string& destCppFile,
		std::ostream& sst) :
		currentIdent(0),
		currentSourceIdent(0),
		stream(st),
		originalFileName(orgFN),
		currentFileName(orgFN),
		classNameToMock(clasName),
		destinationFile(destFile),
		destinationCppFile(destCppFile),
		streamSource(sst),
		csg(nullptr)
	{
	}

	unsigned char currentIdent;
	unsigned char currentSourceIdent;
	std::ostream& stream;
	std::ostream& streamSource;
	std::string originalFileName;
	std::string currentFileName;
	std::string classNameToMock;
	std::string destinationFile;
	std::string destinationCppFile;
	ClassSourceGenerator* csg;
	
	bool doAllClasses() const
	{
		return classNameToMock.empty();
	}
};
