#include <stack>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <expat.h>
#include "xml.h"
#include "element.h"
#include "exception.h"

namespace relax
{

namespace xml
{

/* expat callbacks */
static void startElement(void* userData, const char* name, const char** atts)
{
	std::stack<Element*>* stack = (std::stack<Element*>*) userData;
	
	Element* element = new Element(name);
	
	if (!stack->empty())
		stack->top()->addChild(element);
		
	stack->push(element);
	
	for (int i = 0; atts[i] != NULL; i += 2)
	{
		std::string attrName = atts[i];
		std::string attrValue = atts[i + 1];
		element->setAttribute(attrName, attrValue);
	}
}

static void endElement(void* userData, const char* name)
{
	std::stack<Element*>* stack = (std::stack<Element*>*) userData;
	
	if (stack->size() > 1)
		stack->pop();
}

static void characterData(void* userData, const char* s, int len)
{
	//std::stack<Element*>* stack = (std::stack<Element*>*) userData;
}

static void parseString(const char* xml, void* userData, bool addFakeRoot = false)
{
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetUserData(parser, userData);
	
	int parsed = 1;
	
	if (parsed && addFakeRoot)
	{
		XML_SetStartElementHandler(parser, NULL);
		XML_SetEndElementHandler(parser, NULL);
		XML_SetCharacterDataHandler(parser, NULL);
		parsed = XML_Parse(parser, "<root>", 6, 0);
	}
	
	if (parsed)
	{
		XML_SetStartElementHandler(parser, startElement);
		XML_SetEndElementHandler(parser, endElement);
		XML_SetCharacterDataHandler(parser, characterData);
		parsed = XML_Parse(parser, xml, strlen(xml), !addFakeRoot);
	}
	
	if (parsed && addFakeRoot)
	{
		XML_SetStartElementHandler(parser, NULL);
		XML_SetEndElementHandler(parser, NULL);
		XML_SetCharacterDataHandler(parser, NULL);
		parsed = XML_Parse(parser, "</root>", 7, 1);
	}
	
	XML_ParserFree(parser);
	
	if (!parsed)
	{
		std::stringstream ss;
		ss << "Error while parsing XML: ";
		ss << XML_ErrorString(XML_GetErrorCode(parser));
		ss << " at line ";
		ss << XML_GetCurrentLineNumber(parser);
		throw Exception(ss.str());
	}
}

static void parseFile(const char* fileName, void* userData, bool addFakeRoot = false)
{
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetUserData(parser, userData);
	
	int parsed = 1;
	
	if (parsed && addFakeRoot)
	{
		XML_SetStartElementHandler(parser, NULL);
		XML_SetEndElementHandler(parser, NULL);
		XML_SetCharacterDataHandler(parser, NULL);
		parsed = XML_Parse(parser, "<root>", 6, 0);
	}
	
	if (parsed)
	{
		XML_SetStartElementHandler(parser, startElement);
		XML_SetEndElementHandler(parser, endElement);
		XML_SetCharacterDataHandler(parser, characterData);
		
		FILE* f = fopen(fileName, "r");
		if (f == NULL)
			throw Exception(std::string("Could not open XML file '") + fileName + "'");
		
		char line[1024];
		while (parsed && !feof(f))
		{
			fgets(line, sizeof(line), f);
			parsed = XML_Parse(parser, line, strlen(line), 0);
		}
		
		if (!addFakeRoot)
			XML_Parse(parser, "", 0, 1);
		
		fclose(f);
	}
	
	if (parsed && addFakeRoot)
	{
		XML_SetStartElementHandler(parser, NULL);
		XML_SetEndElementHandler(parser, NULL);
		XML_SetCharacterDataHandler(parser, NULL);
		parsed = parsed && XML_Parse(parser, "</root>", 7, 1);
	}
	
	XML_ParserFree(parser);
	
	if (!parsed)
	{
		std::stringstream ss;
		ss << "Error while parsing XML: ";
		ss << XML_ErrorString(XML_GetErrorCode(parser));
		ss << " at line ";
		ss << XML_GetCurrentLineNumber(parser);
		throw Exception(ss.str());
	}
}

Element* elementFromXML(const char* xml)
{
	std::stack<Element*> stack;
	parseString(xml, &stack);
	return stack.top();
}

void addXML(Element* element, const char* xml)
{
	std::stack<Element*> stack;
	stack.push(element);
	parseString(xml, &stack, true);
}

Element* elementFromXMLFile(const char* fileName)
{
	std::stack<Element*> stack;
	parseFile(fileName, &stack);
	return stack.top();
}

void addXMLFile(Element* element, const char* fileName)
{
	std::stack<Element*> stack;
	stack.push(element);
	parseFile(fileName, &stack, true);
}

}

}


