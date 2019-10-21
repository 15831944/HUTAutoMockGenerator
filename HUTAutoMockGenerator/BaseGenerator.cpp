#include "BaseGenerator.h"
#include "Constants.h"

void BaseGenerator::AddInclude(std::ostream& stream, const std::string& file, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::includeStatementStart
		<< file
		<< HUT_Constants::includeStatementEnd
		<< HUT_Constants::EndLine;
}

void BaseGenerator::addParameter(std::ostream& stream, std::string type, std::string name, bool isLast)
{
	stream << type
		<< HUT_Constants::Space
		<< name;
		if (!isLast)
		{
			stream << HUT_Constants::CommaSpace;
		}
}
