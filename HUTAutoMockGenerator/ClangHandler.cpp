#include "ClangHandler.h"
#include "ClangCallBacks.h"
#include <string>
#include <algorithm>
#include <ctype.h>

ClangHandler* ClangHandler::m_instance = nullptr;

void ClangHandler::processMethod(CXCursor cursor, CXClientData ccd)
{
	CXCursorKind kind = clang_getCursorKind(cursor);
	if (kind == CXCursorKind::CXCursor_CXXMethod ||
		kind == CXCursorKind::CXCursor_FunctionDecl)
	{
		if (!clang_Cursor_isFunctionInlined(cursor))
		{
			CommonData* cd = nullptr;
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
	}
}

void ClangHandler::processClass(CXCursor c, CXClientData ccd)
{
	CommonData* cd = nullptr;
	if (ccd != nullptr)
	{
		cd = (CommonData*)ccd;
	}
	else
	{
		return;
	}

	std::string name = ToString(clang_getCursorSpelling(c));
	if (clang_getCursorKind(c) == CXCursorKind::CXCursor_ClassDecl)
	{
		if (cd->doAllClasses() || name == cd->classNameToMock)
		{
			MockClassGenerator mcg(name);
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
}

void ClangHandler::cleanIndex(CXIndex& index)
{
	if (index != nullptr)
	{
		clang_disposeIndex(index);
		index = nullptr;
	}
}

void ClangHandler::cleanTranslationUnit(CXTranslationUnit& unit)
{
	if (unit != nullptr)
	{
		clang_disposeTranslationUnit(unit);
		unit = nullptr;
	}
}

bool ClangHandler::Parse(std::string file, int argc, char* argv[], CXIndex& index, CXTranslationUnit& unit)
{
	CXErrorCode er = clang_parseTranslationUnit2(
		index,
		file.c_str(),
		argv,
		argc,
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
		std::cerr << "Unable to parse the input file " << er << std::endl;
		return false;
	}
	return true;
}

bool ClangHandler::CheckUnknownTypesAndFix(CXTranslationUnit& unit, CXIndex& index, std::string file, int argc, char* argv[], int remaining)
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
				ParseAndCheck(newFile, argc, argv, index, unit, remaining);
				std::remove(newFile.c_str());
			}
		}
	}
	return true;
}

bool ClangHandler::ParseAndCheck(std::string fileName, int argc, char* argv[], CXIndex& index, CXTranslationUnit& unit, int remaining)
{
	if (!Parse(fileName, argc, argv, index, unit))
	{
		cleanIndex(index);
		return false;
	}
	if (!CheckUnknownTypesAndFix(unit, index, fileName, argc, argv, remaining))
	{
		cleanTranslationUnit(unit);
		cleanIndex(index);
		return false;
	}
	return true;
}

char params[1][500] = {
	"-ferror-limit=10000"
};

int ClangHandler::Process(int argc, char* argv[])
{
	std::string fileName(argv[1]);
	std::string classToFind(argv[2]);
	std::string temp(classToFind.length(), 0);
	std::transform(classToFind.begin(), classToFind.end(), temp.begin(), ::tolower);
	if (temp.compare("all") == 0)
	{
		classToFind = "";
	}
	int offset = 4;
	int newargc = argc - offset + 1;
	char** newargv = new char* [newargc];
	for (auto i = offset; i < argc; ++i)
	{
		newargv[i - offset] = argv[i];
	}
	newargv[newargc - 1] = params[0];

	std::string destinationFile(argv[3]);
	std::ofstream out(destinationFile);

	if (!out.is_open())
	{
		std::cerr << "Failed to create destination file, please check you have write access to the location" << std::endl;
		return -1;
	}
	
	CommonData cd(out, fileName, classToFind, destinationFile);
	MockClassGenerator mcg("");
	mcg.AddInclude(cd.stream, HUT_Constants::gmockHeader, cd.currentIdent);
	mcg.AddInclude(cd.stream, GetFileNameFromPath(cd.originalFileName), cd.currentIdent);
	
	CXIndex index = clang_createIndex(0, 0);
	CXTranslationUnit unit;
	
	if (ParseAndCheck(fileName, newargc, newargv, index, unit))
	{
		CXCursor cursor = clang_getTranslationUnitCursor(unit);

		clang_visitChildren(cursor, classVisitor, &cd);
	}

	cleanTranslationUnit(unit);
	cleanIndex(index);

	return 0;
}
