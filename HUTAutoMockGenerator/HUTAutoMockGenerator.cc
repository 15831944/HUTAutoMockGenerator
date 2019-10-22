#include "HUTAutoMockGenerator.h"
#include "ClangHandler.h"

const std::string sourceFile = "--source-file=";
const std::string class_tag = "--class=";
const std::string destinationFolder = "--destination-folder=";
const std::string help = "--help";

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
			<< "\"" << sourceFile << "<file to parse>\" " 			
			<< "\"" << class_tag << "<class to mock or ALL>\" " 			
			<< "\"" << destinationFolder << "<destination path>\" "
			<< "<other clang compiler options eg -D, -I etc>"
			<< std::endl << std::endl;
}

int HUTAutoMockGenerator(int argc, char* argv[])
{
	if ((argc < 2 && std::string(argv[1]).compare(help) == 0) 
		||
		argc < 4 
		||
		!(
			std::string(argv[1]).find(sourceFile) == 0 
			&& std::string(argv[1]).length() > sourceFile.length()) 
		||
		!(
			std::string(argv[2]).find(class_tag) == 0 
			&& std::string(argv[2]).length() > class_tag.length()) 
		||
		!(
			std::string(argv[3]).find(destinationFolder) == 0 
			&& std::string(argv[3]).length() > destinationFolder.length())
		)
	{
		printHelp();
		return -1;
	}

	leftShiftString(argv[1], sourceFile.length());
	leftShiftString(argv[2], class_tag.length());
	leftShiftString(argv[3], destinationFolder.length());

	ClangHandler::getInstance()->Process(argc, argv);
	return 0;
}
