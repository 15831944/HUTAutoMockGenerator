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

void print_function_prototype(CXCursor cursor, CXClientData ccd)
{
	CommonData *cd = nullptr;
	if (ccd != nullptr)
	{
		cd = (CommonData*)ccd;
	}
	else
	{
		return;
	}

	auto type = clang_getCursorType(cursor);
	
	auto function_name = ToString(clang_getCursorSpelling(cursor));
	auto return_type = ToString(clang_getTypeSpelling(clang_getResultType(type)));
	int num_args = clang_Cursor_getNumArguments(cursor);
	MockMethodGenerator mmg;
	mmg.startMockMethod(cd->stream, function_name, num_args, return_type, clang_CXXMethod_isConst(cursor), cd->currentIdent);

	for (int i = 0; i < num_args; ++i)
	{
		auto arg_cursor = clang_Cursor_getArgument(cursor, i);
		auto arg_name = ToString(clang_getCursorSpelling(arg_cursor));
		
		auto arg_data_type = ToString(clang_getTypeSpelling(clang_getArgType(type, i)));
		mmg.addParameter(cd->stream, arg_data_type, arg_name, i == num_args - 1);
	}

	mmg.endMockMethod(cd->stream);
}

CXChildVisitResult method_visitor(CXCursor c, CXCursor p, CXClientData cd)
{
	CXCursorKind kind = clang_getCursorKind(c);
	if (kind == CXCursorKind::CXCursor_CXXMethod ||
		kind == CXCursorKind::CXCursor_FunctionDecl)
	{
		if (!clang_Cursor_isFunctionInlined(c))
		{
			print_function_prototype(c, cd);
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
	"-DRN1UTILS_API=__declspec(dllimport)",
	"-ferror-limit=1000"
};

void cleanIndex(CXIndex& index)
{
	if (index != nullptr)
	{
		clang_disposeIndex(index);
		index = nullptr;
	}
}

void cleanTranslationUnit(CXTranslationUnit& unit)
{
	if (unit != nullptr)
	{
		clang_disposeTranslationUnit(unit);
		unit = nullptr;
	}
}

bool Parse(std::string file, CXIndex& index, CXTranslationUnit* unit)
{
	CXErrorCode er = clang_parseTranslationUnit2(
		index,
		file.c_str(),
		params,
		2,
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
		unit
	);

	if (unit == nullptr || er != CXErrorCode::CXError_Success)
	{
		std::cerr << "Unable to parse the input file" << std::endl;
		return false;
	}
	return true;
}

bool CheckUnknownTypesAndFix(CXTranslationUnit& unit, CXIndex& index, std::string file, int remaining = 3)
{
	if (remaining == 0)
	{
		return false;
	}

	--remaining;
	auto count = clang_getNumDiagnostics(unit);
	std::set<std::string> errors;
	if (count != 0)
	{
		for (unsigned int i = 0; i < count; ++i)
		{
			CXDiagnostic diag = clang_getDiagnostic(unit, i);
			std::string val = ToString(clang_getDiagnosticSpelling(diag));
			if (val.find("unknown type name") != std::string::npos)
			{
				auto pos1 = val.find('\'');
				auto pos2 = val.rfind('\'');
				if (pos1 != std::string::npos && pos2 != std::string::npos)
				{
					errors.insert(val.substr(pos1 + 1, pos2 - pos1 - 1));
				}
			}
			clang_disposeDiagnostic(diag);
		}

		std::string lineToInsert = "";
		for (auto itr : errors)
		{
			lineToInsert = lineToInsert + "typedef int " + itr + ";\n";
		}
		if (!lineToInsert.empty())
		{
			cleanTranslationUnit(unit);

			time_t now = time(0);
			tm* ltm = localtime(&now);
			char buffer[80];
			strftime(buffer, 80, "%d%B%Y%H%M%S", ltm);
			std::string newFile = file;
			newFile.insert(newFile.find('.'), buffer);
			std::cout << newFile << std::endl;
			std::ifstream in(file);
			std::ofstream out(newFile);
			if (in.is_open() && out.is_open())
			{
				out << lineToInsert;
				std::string line;
				while (std::getline(in, line))
				{
					out << line << "\n";
				}
				in.close();
				out.close();
				if (!Parse(newFile, index, &unit))
				{
					cleanIndex(index);
					return false;
				}
				if (!CheckUnknownTypesAndFix(unit, index, newFile, remaining))
				{
					return false;
				}
			}
		}
	}
	return true;
}

CXChildVisitResult classVisitor(CXCursor c, CXCursor p, CXClientData ccd)
{	
	CommonData* cd = nullptr;
	if (ccd != nullptr)
	{
		cd = (CommonData*)ccd;
	}
	else
	{
		return CXChildVisit_Recurse;
	}

	std::string name = ToString(clang_getCursorSpelling(c));
	if (clang_getCursorKind(c) == CXCursorKind::CXCursor_ClassDecl)
	{
		if (!cd->doAllClasses() && name == cd->classNameToMock)
		{
			MockClassGenerator mcg(name);
			std::string newName = mcg.getMockClassName();
			std::string statPointer = mcg.getInternalPointer();
			mcg.AddInclude(cd->stream, HUT_Constants::gmockHeader, cd->currentIdent);
			mcg.AddInclude(cd->stream, GetFileNameFromPath(fileName), cd->currentIdent);
			mcg.addEmptyLine(cd->stream);
			mcg.startClassDefinition(cd->stream, cd->currentIdent);
			cd->currentIdent += 1;
			mcg.addMockInternalPointer(cd->stream, cd->currentIdent);
			mcg.addMockConstructor(cd->stream, cd->currentIdent);
			mcg.addMockDestructor(cd->stream, cd->currentIdent);
			mcg.addEmptyLine(cd->stream);

			clang_visitChildren(c, method_visitor, cd);

			cd->currentIdent -= 1;
			mcg.endClassDefinition(cd->stream, cd->currentIdent);
		}
	}
	return CXChildVisit_Recurse;
}

int main()
{
	CommonData cd(std::cout, fileName, "CRRecordSet");
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit;
	if (!Parse(fileName, index, &unit))
	{
		cleanIndex(index);
		return -1;
	}
	if (!CheckUnknownTypesAndFix(unit, index, fileName))
	{
		cleanTranslationUnit(unit);
		cleanIndex(index);
		return -1;
	}
	
	CXCursor cursor = clang_getTranslationUnitCursor(unit);

	clang_visitChildren(cursor, classVisitor, &cd);

	cleanTranslationUnit(unit);
	cleanIndex(index);
	
	return 0;
}
