#include "MockClassGenerator.h"
#include "Constants.h"

MockClassGenerator::MockClassGenerator(std::string org_className)
{
	m_mockClassName = HUT_Constants::prefixToClassName + org_className;
	m_mockInternalPointer = HUT_Constants::prefixToInternalPointer + m_mockClassName;
}

void MockClassGenerator::startClassDefinition(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::classStart << HUT_Constants::Space << getMockClassName()
		<< HUT_Constants::EndLine
		<< HUT_Constants::openCurlyBracket
		<< HUT_Constants::EndLine
		<< HUT_Constants::publicAccessSpecifier
		<< HUT_Constants::EndLine;
}

void MockClassGenerator::addMockInternalPointer(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::Static << HUT_Constants::Space
		<< getMockClassName()
		<< HUT_Constants::Space << HUT_Constants::Pointer
		<< getInternalPointer()
		<< HUT_Constants::EndStatment
		<< HUT_Constants::EndLine;
}

void MockClassGenerator::addMockConstructor(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< getMockClassName()
		<< HUT_Constants::OpenParanthesis
		<< HUT_Constants::CloseParanthesis
		<< HUT_Constants::EndLine
		<< tabIndent(level)
		<< HUT_Constants::openCurlyBracket
		<< HUT_Constants::EndLine
		<< tabIndent(level + 1)
		<< getInternalPointer()
		<< HUT_Constants::Assignment
		<< HUT_Constants::This
		<< HUT_Constants::EndStatment
		<< HUT_Constants::EndLine
		<< tabIndent(level)
		<< HUT_Constants::closeCurlyBracket
		<< HUT_Constants::EndLine;
}

void MockClassGenerator::addMockDestructor(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::DestructorPreFix
		<< getMockClassName()
		<< HUT_Constants::OpenParanthesis
		<< HUT_Constants::CloseParanthesis
		<< HUT_Constants::EndLine
		<< tabIndent(level)
		<< HUT_Constants::openCurlyBracket
		<< HUT_Constants::EndLine
		<< tabIndent(level + 1)
		<< getInternalPointer()
		<< HUT_Constants::Assignment
		<< HUT_Constants::NullPtr
		<< HUT_Constants::EndStatment
		<< HUT_Constants::EndLine
		<< tabIndent(level)
		<< HUT_Constants::closeCurlyBracket
		<< HUT_Constants::EndLine;
}

void MockClassGenerator::endClassDefinition(std::ostream& stream, int level)
{
	stream << tabIndent(level)
		<< HUT_Constants::classEnd
		<< HUT_Constants::EndStatment
		<< HUT_Constants::EndLine;
}
