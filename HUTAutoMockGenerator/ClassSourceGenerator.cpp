#include "ClassSourceGenerator.h"
#include "Constants.h"

void ClassSourceGenerator::defineMockClassPointer(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< m_mockClassName
		<< HUT_Constants::Pointer
		<< HUT_Constants::Space
		<< m_mockClassName
		<< HUT_Constants::ScopeResolutionOperator
		<< m_mockClassPointer
		<< HUT_Constants::Assignment
		<< HUT_Constants::NullPtr
		<< HUT_Constants::EndStatment
		<< HUT_Constants::EndLine
		<< HUT_Constants::EndLine;
}

void ClassSourceGenerator::startMethodDefinition(std::ostream& stream, const std::string& returnType, const std::string& methodName, int level)
{
	params.clear();
	m_methodName = methodName;
	stream << tabIndent(level)
		<< returnType
		<< HUT_Constants::Space
		<< m_className
		<< HUT_Constants::ScopeResolutionOperator
		<< methodName
		<< HUT_Constants::OpenParanthesis;
}

void ClassSourceGenerator::endMethodDefinition(std::ostream& stream, bool isConstant, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::CloseParanthesis
		<< HUT_Constants::Space
		<< (isConstant ? HUT_Constants::Constant : HUT_Constants::EmptyString)
		<< HUT_Constants::EndLine;
}

void ClassSourceGenerator::startMethodBody(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::openCurlyBracket
		<< HUT_Constants::EndLine;
}

void ClassSourceGenerator::generateMethodBody(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::Return
		<< HUT_Constants::Space
		<< m_mockClassName
		<< HUT_Constants::ScopeResolutionOperator
		<< m_mockClassPointer
		<< HUT_Constants::PointerDirectOperator
		<< m_methodName
		<< HUT_Constants::OpenParanthesis;
	for (auto i = 0; i < params.size(); ++i)
	{
		stream << params[i];
		if (i != params.size() - 1) 
		{
			stream << HUT_Constants::CommaSpace;
		}
	}
	stream << HUT_Constants::CloseParanthesis
		<< HUT_Constants::EndStatment
		<< HUT_Constants::EndLine;
}

void ClassSourceGenerator::endMethodBody(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::closeCurlyBracket
		<< HUT_Constants::EndLine
		<< HUT_Constants::EndLine;
}

void ClassSourceGenerator::addParameter(std::ostream& stream, std::string type, std::string name, bool isLast)
{
	params.push_back(name);
	BaseGenerator::addParameter(stream, type, name, isLast);
}
