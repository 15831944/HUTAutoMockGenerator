#include "ClangCallBacks.h"
#include "ClangHandler.h"

CXChildVisitResult classVisitor(CXCursor c, CXCursor p, CXClientData ccd)
{
	ClangHandler::getInstance()->processClass(c, ccd);
	return CXChildVisit_Recurse;
}

CXChildVisitResult method_visitor(CXCursor c, CXCursor p, CXClientData cd)
{
	ClangHandler::getInstance()->processMethod(c, cd);
	return CXChildVisit_Continue;
}

//
//
//CXVisitorResult include_visitor(void* context, CXCursor cursor, CXSourceRange range) {
//
//	CXFile file = clang_getIncludedFile(cursor);
//	std::string filename = ToString(clang_getFileName(file));
//	if (filename.find("Visual Studio") != std::string::npos)
//	{
//		filename = filename.substr(filename.find_last_of("\\") + 1);
//		std::cout << "#include <" << filename << ">" << std::endl;
//	}
//	else
//	{
//		std::cout << "#include \"" << filename << "\"" << std::endl;
//	}
//	return CXVisit_Continue;
//}
//
//CXCursorAndRangeVisitor visitor = {
//  0,
//  include_visitor
//};
//
//void showInclusions(CXTranslationUnit TU, const char* src_filename) {
//
//	CXFile file = clang_getFile(TU, src_filename);
//
//	CXResult result = clang_findIncludesInFile(TU, file, visitor);
//}
