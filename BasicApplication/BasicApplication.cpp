#include <iostream>
#include "HUTAutoMockGenerator.h"

int main()
{
	char str1[1024] = "--source-file=D:\\rrecord.h";
	char str2[1024] = "--class=all";
	char str3[1024] = "--destination-folder=C:\\temp";
	char str4[1024] = "-DRN1UTILS_API=";

	int argc = 4;
	char** argv = new char* [argc];
	argv[0] = new char[1024];
	argv[1] = new char[1024];
	argv[2] = new char[1024];
	argv[3] = new char[1024];
	memset(argv[0], 0, 1024);
	memset(argv[1], 0, 1024);
	memset(argv[2], 0, 1024);
	memset(argv[3], 0, 1024);
	memcpy(argv[0], str1, strlen(str1));
	memcpy(argv[1], str2, strlen(str2));
	memcpy(argv[2], str3, strlen(str3));
	memcpy(argv[3], str4, strlen(str4));

	return HUTAutoMockGenerator(argc, argv);
}

