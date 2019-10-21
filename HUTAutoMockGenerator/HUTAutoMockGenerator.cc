#include "HUTAutoMockGenerator.h"
#include "ClangHandler.h"

int HUTAutoMockGenerator(int argc, char* argv[])
{
	if (argc < 5)
	{
		std::cerr << "invalid format" << std::endl
			<< "Usage:" << std::endl
			<< "\t HUTAutoMockGenerator.exe <filename to parse> <class to find> <folder to output>" << std::endl << std::endl
			<< "**Note: want all classes to mock => <class to find> = all" << std::endl;
		return -1;
	}
	ClangHandler::getInstance()->Process(argc, argv);
	return 0;
}
