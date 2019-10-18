#include "MockMethodGenerator.h"
#include "Constants.h"

void MockMethodGenerator::startMockMethod(std::ostream& stream, std::string name, int num_args, std::string return_type, bool isConst, int level)
{
	stream << tabIndent(level)
		<< ((num_args > 10) ? HUT_Constants::COMMENT_SL : HUT_Constants::EmptyString)
		<< (isConst ? HUT_Constants::MOCK_CONST_METHOD : HUT_Constants::MOCK_METHOD)
		<< num_args
		<< HUT_Constants::OpenParanthesis
		<< name
		<< HUT_Constants::CommaSpace
		<< return_type
		<< HUT_Constants::OpenParanthesis;
}

void MockMethodGenerator::addParameter(std::ostream& stream, std::string type, std::string name, bool isLast)
{
	stream << type
		<< HUT_Constants::Space
		<< name
		<< (isLast ? HUT_Constants::EmptyString : HUT_Constants::CommaSpace);
}

void MockMethodGenerator::endMockMethod(std::ostream& stream)
{
	stream << HUT_Constants::CloseParanthesis
		<< HUT_Constants::EndMockMethod
		<< HUT_Constants::EndStatment
		<< HUT_Constants::EndLine;
}
