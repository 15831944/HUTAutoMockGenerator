#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <clang-c/Index.h>
#include <set>
#include <chrono>
#include <fstream>

#include "Utils.h"
#include "MockClassGenerator.h"
#include "MockMethodGenerator.h"
#include "Constants.h"
#include "CommonData.h"

class ClangHandler
{
private:
	ClangHandler() {}
	static ClangHandler* m_instance;

public:
	static ClangHandler* getInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new ClangHandler();
		}
		return m_instance;
	}

	int Process(int arc, char* argv[]);
	bool ParseAndCheck(std::string file, int argc, char* argv[], CXIndex& index, CXTranslationUnit& unit, int remaining = 3);
	bool Parse(std::string file, int argc, char* argv[], CXIndex& index, CXTranslationUnit& unit);
	bool CheckUnknownTypesAndFix(CXTranslationUnit& unit, CXIndex& index, std::string file, int argc, char* argv[], int remaining = 3);
	void cleanTranslationUnit(CXTranslationUnit& unit);
	void cleanIndex(CXIndex& index);

	void processMethod(CXCursor cursor, CXClientData ccd);
	void processClass(CXCursor cursor, CXClientData ccd);
};

