#include "HUTAutoMockGenerator.h"
#include "ClangHandler.h"

void leftShiftString(char* str, int len)
{
	int org_len = strlen(str);
	auto i = 0;
	while (i < org_len - len)
	{
		str[i] = str[len + i];
		++i;
	}
	while (i < org_len)
	{
		str[i] = '\0';
		++i;
	}
}

void printHelp()
{
	std::cout << "Invalid input arguments" << std::endl
		<< "Usage:" << std::endl << std::endl
		<< "HUTAutoMockGenerator.exe " 
			<< "\"" << sourceFileTag << "<file to parse>\" " 			
			<< "\"" << classTag << "<class to mock or ALL>\" " 			
			<< "\"" << destinationFolderTag << "<destination path>\" "
			<< "<other clang compiler options eg -D, -I etc>"
			<< std::endl << std::endl;
}

int HUTAutoMockGenerator(int argc, char* argv[])
{
	if ((argc < 1 && std::string(argv[0]).compare(help) == 0) 
		||
		argc < 3 
		||
		!(
			std::string(argv[0]).find(sourceFileTag) == 0 
			&& std::string(argv[0]).length() > sourceFileTag.length()) 
		||
		!(
			std::string(argv[1]).find(classTag) == 0 
			&& std::string(argv[1]).length() > classTag.length()) 
		||
		!(
			std::string(argv[2]).find(destinationFolderTag) == 0 
			&& std::string(argv[2]).length() > destinationFolderTag.length())
		)
	{
		printHelp();
		return -1;
	}

	leftShiftString(argv[0], sourceFileTag.length());
	leftShiftString(argv[1], classTag.length());
	leftShiftString(argv[2], destinationFolderTag.length());

	ClangHandler::getInstance()->Process(argc, argv);
	return 0;
}
