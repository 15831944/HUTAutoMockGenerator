#pragma once
#include <iostream>
#include <sstream>
#include "Utils.h"
#include <vector>
#include "BaseGenerator.h"

class ClassSourceGenerator : public BaseGenerator
{
private:
	std::string m_className;
	std::string m_mockClassName;
	std::string m_mockClassPointer;
	std::vector<std::string> params;
	std::string m_methodName;
	
public:
	ClassSourceGenerator(const std::string& cn,
		const std::string& mcn,
		const std::string& mcp) : 
		m_className(cn),
		m_mockClassName(mcn),
		m_mockClassPointer(mcp),
		m_methodName("")
	{
		params.clear();
	}

	void defineMockClassPointer(std::ostream& stream, int level = 0);
	void startMethodDefinition(std::ostream& stream, const std::string& returnType,
		const std::string& methodName, int level = 0);
	void endMethodDefinition(std::ostream& stream, bool isConstant, int level = 0);
	void startMethodBody(std::ostream& stream, int level = 0);
	void generateMethodBody(std::ostream& stream, int level = 0);
	void endMethodBody(std::ostream& stream, int level = 0);
	virtual void addParameter(std::ostream& stream, std::string type, std::string name, bool isLast = false) override;
};

