#include "ClangHandler.h"

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cerr << "invalid format" << std::endl
			<< "Usage:" << std::endl
			<< "\t HUTAutoMockGenerator.exe <filename to parse> <class to find>" << std::endl
			<< "**Note: want all classes to mock => <class to find> = all" << std::endl;
		return -1;
	}
	ClangHandler::getInstance()->Process(argc, argv);
	return 0;
}
