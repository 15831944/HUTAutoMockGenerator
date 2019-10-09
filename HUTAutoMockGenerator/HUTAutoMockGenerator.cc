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

CXChildVisitResult method_visitor(CXCursor c, CXCursor p, CXClientData cd)
{
	CXCursorKind kind = clang_getCursorKind(c);
	//std::cout << "###" << ToString(clang_getCursorSpelling(c)) << "%%%%"
	//	<< clang_getCursorKindSpelling(clang_getCursorKind(c))  << std::endl;
	if (kind == CXCursorKind::CXCursor_CXXMethod ||
		kind == CXCursorKind::CXCursor_FunctionDecl)
	{
		if (!clang_Cursor_isFunctionInlined(c))
		{
			print_function_prototype(c);
		}
	}
	return CXChildVisit_Continue;
}

CXVisitorResult include_visitor(void* context, CXCursor cursor, CXSourceRange range) {

	CXFile file = clang_getIncludedFile(cursor);
	std::string filename = ToString(clang_getFileName(file));
	if (filename.find("Visual Studio") != std::string::npos)
	{
		filename = filename.substr(filename.find_last_of("\\") + 1);
		std::cout << "#include <" << filename << ">" << std::endl;
	}
	else
	{
		std::cout << "#include \"" << filename << "\"" << std::endl;
	}
	return CXVisit_Continue;
}

std::string GetFileNameFromPath(const std::string& f)
{
	if (f.find_last_of("\\") != std::string::npos)
	{
		return f.substr(f.find_last_of("\\") + 1);
	}
	return f;
}

CXCursorAndRangeVisitor visitor = {
  0,
  include_visitor
};

void showInclusions(CXTranslationUnit TU, const char* src_filename) {

	CXFile file = clang_getFile(TU, src_filename);
	
	CXResult result = clang_findIncludesInFile(TU, file, visitor);
}

std::string fileName = "TestData\\test.hpp";
const char* const params[] = {
	"-DRN1UTILS_API=__declspec(dllimport)"
};

int main()
{
	CXIndex index = clang_createIndex(0, 1);
	CXTranslationUnit unit;
		
	CXErrorCode er = clang_parseTranslationUnit2(
		index,
		fileName.c_str(),
		params,
		1,
		nullptr,
		0,
		CXTranslationUnit_DetailedPreprocessingRecord |
		CXTranslationUnit_Incomplete |
		CXTranslationUnit_PrecompiledPreamble |
		CXTranslationUnit_CacheCompletionResults |
		CXTranslationUnit_ForSerialization |
		CXTranslationUnit_CXXChainedPCH |
		CXTranslationUnit_SkipFunctionBodies |
		CXTranslationUnit_IncludeBriefCommentsInCodeCompletion |
		CXTranslationUnit_CreatePreambleOnFirstParse |
		CXTranslationUnit_KeepGoing |
		CXTranslationUnit_SingleFileParse |
		CXTranslationUnit_LimitSkipFunctionBodiesToPreamble |
		CXTranslationUnit_IncludeAttributedTypes |
		CXTranslationUnit_VisitImplicitAttributes |
		CXTranslationUnit_IgnoreNonErrorsFromIncludedFiles,
		&unit
	);

	if (unit == nullptr || er != CXErrorCode::CXError_Success)
	{
		std::cerr << "Unable to parse the input file" << std::endl;
		return -1;
	}

	//showInclusions(unit, fileName.c_str());
	
	CXCursor cursor = clang_getTranslationUnitCursor(unit);
	clang_visitChildren(cursor,
		[](CXCursor c, CXCursor p, CXClientData cd)
		{
			std::string name = ToString(clang_getCursorSpelling(c));
			//std::cout << name << std::endl;

			if (clang_getCursorKind(c) == CXCursorKind::CXCursor_ClassDecl)
			{
				if (name == "CRRecordSet")
				{
					std::string newName = "Mock" + name;
					std::string statPointer = "p" + newName;
					std::cout << "#include \"gmock/gmock.h\"" << std::endl;
					std::cout << "#include \"" << GetFileNameFromPath(fileName) << "\"" << std::endl;
					std::cout << std::endl;
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

					clang_visitChildren(c, method_visitor, nullptr);
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
