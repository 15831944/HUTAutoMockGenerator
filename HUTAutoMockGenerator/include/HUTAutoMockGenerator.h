#pragma once
#ifdef HUTAUTOMOCKGENERATOR_EXPORTS
#define HUTAUTOMOCKGENERATOR_API __declspec(dllexport)
#else
#define HUTAUTOMOCKGENERATOR_API __declspec(dllimport)
#endif
#include <iostream>
#include <string>

const std::string sourceFileTag = "--source-file=";
const std::string classTag = "--class=";
const std::string destinationFolderTag = "--destination-folder=";
const std::string help = "--help";

HUTAUTOMOCKGENERATOR_API int HUTAutoMockGenerator(int argc, char* argv[]);
