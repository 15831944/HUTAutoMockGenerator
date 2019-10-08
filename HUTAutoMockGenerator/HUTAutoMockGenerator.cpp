#include <iostream>
#include <clang-c/Index.h>

std::ostream& operator<<(std::ostream& stream, const CXString& str)
{
	stream << clang_getCString(str);
	clang_disposeString(str);
	return stream;
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
		0,
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
			std::cout << "Cursor '" << clang_getCursorSpelling(c) << "' of kind '"
				<< clang_getCursorKindSpelling(clang_getCursorKind(c)) << "'\n";
			return CXChildVisit_Recurse;
		},
		nullptr);

	clang_disposeTranslationUnit(unit);
	clang_disposeIndex(index);
	return 0;
}
