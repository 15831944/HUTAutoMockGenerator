#pragma once
#ifdef HUTAUTOMOCKGENERATOR_EXPORTS
#define HUTAUTOMOCKGENERATOR_API __declspec(dllexport)
#else
#define HUTAUTOMOCKGENERATOR_API __declspec(dllimport)
#endif

HUTAUTOMOCKGENERATOR_API int HUTAutoMockGenerator(int argc, char* argv[]);
