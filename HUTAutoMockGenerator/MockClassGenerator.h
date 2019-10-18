#pragma once
#include <iostream>
#include <sstream>
#include "Utils.h"

class MockClassGenerator
{
private:
	std::string m_mockClassName;
	std::string m_mockInternalPointer;

public:
	explicit MockClassGenerator(std::string org_className);

	constexpr const std::string& getMockClassName() const noexcept
	{
		return m_mockClassName;
	}

	constexpr const std::string& getInternalPointer() const noexcept
	{
		return m_mockInternalPointer;
	}

	void addEmptyLine(std::ostream& stream) { stream << std::endl; }
	void AddInclude(std::ostream& stream, const std::string& file, int level = 0);
	void startClassDefinition(std::ostream& stream, int level = 0);
	void addMockInternalPointer(std::ostream& stream, int level = 0);
	void addMockConstructor(std::ostream& stream, int level = 0);
	void addMockDestructor(std::ostream& stream, int level = 0);
	void endClassDefinition(std::ostream& stream, int level = 0);
};
