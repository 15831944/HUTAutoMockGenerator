#include <iostream>
#include <string>
#include <sstream>
#include <clang-c/Index.h>

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

void parseUsrString(const std::string& usrString, bool* isConst) {
	size_t bangLocation = usrString.find("#&");
	if (bangLocation == std::string::npos || bangLocation == usrString.length() - 1) {
		*isConst = false;
		return;
	}
	bangLocation += 2;
	int x = usrString[bangLocation];

	*isConst = x & 0x1;
}

void print_function_prototype(CXCursor cursor)
{
	auto type = clang_getCursorType(cursor);
	
	auto function_name = ToString(clang_getCursorSpelling(cursor));
	auto return_type = ToString(clang_getTypeSpelling(clang_getResultType(type)));
	CXString usr = clang_getCursorUSR(cursor);
	std::string usr_string = ToString(usr);
	bool isConst = false;
	parseUsrString(usr_string, &isConst);
	std::string MockMacro = "MOCK_METHOD";
	if (isConst)
	{
		MockMacro = "MOCK_CONST_METHOD";
	}
	int num_args = clang_Cursor_getNumArguments(cursor);
	std::cout << "\t" << MockMacro << num_args << "("
		<< function_name << ", " << return_type << "(";
	for (int i = 0; i < num_args; ++i)
	{
		auto arg_cursor = clang_Cursor_getArgument(cursor, i);
		auto arg_name = ToString(clang_getCursorSpelling(arg_cursor));
		if (arg_name.empty())
		{
			arg_name = "no name!";
		}

		auto arg_data_type = ToString(clang_getTypeSpelling(clang_getArgType(type, i)));
		std::cout << arg_data_type << " " << arg_name;
		if (i != num_args - 1)
		{
			std::cout << ", ";
		}
	}

	std::cout << "));" << std::endl;
}

CXChildVisitResult FindMethods(CXCursor c, CXCursor p, CXClientData cd)
{
	CXCursorKind kind = clang_getCursorKind(c);
	if (kind == CXCursorKind::CXCursor_CXXMethod ||
		kind == CXCursorKind::CXCursor_FunctionDecl)
	{
		
		print_function_prototype(c);
	}
	return CXChildVisit_Continue;
}

int main()
{
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit;
		
	CXErrorCode er = clang_parseTranslationUnit2(
		index,
		"TestData\\test.hpp",
		nullptr,
		0,
		nullptr,
		0,
		CXTranslationUnit_SkipFunctionBodies,
		&unit
	);

	if (unit == nullptr || er != CXErrorCode::CXError_Success)
	{
		std::cerr << "Unable to parse the input file" << std::endl;
		return -1;
	}

	CXCursor cursor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(cursor,
		[](CXCursor c, CXCursor p, CXClientData cd)
		{
			if (clang_getCursorKind(c) == CXCursorKind::CXCursor_ClassDecl)
			{
				std::string name = ToString(clang_getCursorSpelling(c));
				if (name == "Test")
				{
					std::string newName = "Mock" + name;
					std::string statPointer = "p" + newName;
					std::cout << "class Mock" << name << std::endl;
					std::cout << "{" << std::endl;
					std::cout << "public:" << std::endl;
					std::cout << "\t" << "static " << newName << "* " << statPointer << ";" << std::endl;
					std::cout << "\t" << newName << "()" << std::endl;
					std::cout << "\t" << "{" << std::endl;
					std::cout << "\t\t" << statPointer << " = this;" << std::endl;
					std::cout << "\t" << "}" << std::endl;
					std::cout << std::endl;
					std::cout << "\t" << "~" << newName << "()" << std::endl;
					std::cout << "\t" << "{" << std::endl;
					std::cout << "\t\t" << statPointer << " = nullptr;" << std::endl;
					std::cout << "\t" << "}" << std::endl;
					std::cout << std::endl;

					clang_visitChildren(c, FindMethods, nullptr);
					std::cout << "};" << std::endl;
				}
			}
			return CXChildVisit_Recurse;
		},
		nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return 0;
}
