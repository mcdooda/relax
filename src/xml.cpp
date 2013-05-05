#include <stack>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <expat.h>
#include "xml.h"
#include "element.h"
#include "text.h"
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
	std::string str(s, len);
	str = Text::trim(str);
	if (str != "")
	{
		std::stack<Element*>* stack = (std::stack<Element*>*) userData;
		
		Element* element = new Element("%string");
		
		Font* font = Font::getDefault();
		
		if (!stack->empty())
		{
			Element* top = stack->top();
			top->addChild(element);
			font = top->getFont();
		}
		
		Color color(0, 0, 0, 255);

		const Rectangle& rectangle = element->getRectangle();
		Text* text = new Text(font, color, rectangle);
		text->appendString(str);
		
		Background* background = new Background(text);
		element->setBackground(background);
	}
}

static void parseString(const char* xml, void* userData, bool addFakeRoot = false)
{
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetUserData(parser, userData);
	
	try {
	
		int parsed = 1;
	
		if (addFakeRoot)
		{
			XML_SetStartElementHandler(parser, NULL);
			XML_SetEndElementHandler(parser, NULL);
			XML_SetCharacterDataHandler(parser, NULL);
			parsed = XML_Parse(parser, "<root>", 6, 0);
			
			if (!parsed)
				throw Exception();
		}
	
		XML_SetStartElementHandler(parser, startElement);
		XML_SetEndElementHandler(parser, endElement);
		XML_SetCharacterDataHandler(parser, characterData);
		parsed = XML_Parse(parser, xml, strlen(xml), !addFakeRoot);
		
		if (!parsed)
			throw Exception();
	
		if (addFakeRoot)
		{
			XML_SetStartElementHandler(parser, NULL);
			XML_SetEndElementHandler(parser, NULL);
			XML_SetCharacterDataHandler(parser, NULL);
			parsed = XML_Parse(parser, "</root>", 7, 1);
			
			if (!parsed)
				throw Exception();
		}
	
	}
	catch (Exception& ex)
	{
		XML_ParserFree(parser);
		
		std::stringstream ss;
		if (ex.hasMessage())
		{
			ss << ex.getMessage();
		}
		else
		{
			ss << "Error while parsing XML: ";
			ss << XML_ErrorString(XML_GetErrorCode(parser));
		}
		ss << " at line ";
		ss << XML_GetCurrentLineNumber(parser);
		throw Exception(ss.str());
	}
	
	XML_ParserFree(parser);
}

static void parseFile(const char* fileName, void* userData, bool addFakeRoot = false)
{
	XML_Parser parser = XML_ParserCreate(NULL);
	XML_SetUserData(parser, userData);
	
	try {
	
		int parsed = 1;
	
		if (addFakeRoot)
		{
			XML_SetStartElementHandler(parser, NULL);
			XML_SetEndElementHandler(parser, NULL);
			XML_SetCharacterDataHandler(parser, NULL);
			parsed = XML_Parse(parser, "<root>", 6, 0);
			
			if (!parsed)
				throw Exception();
		}
	
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
			if (!feof(f))
			{
				parsed = XML_Parse(parser, line, strlen(line), 0);
				if (!parsed)
					throw Exception();
			}
		}
		
		if (!addFakeRoot)
			XML_Parse(parser, "", 0, 1);
		
		fclose(f);
	
		if (addFakeRoot)
		{
			XML_SetStartElementHandler(parser, NULL);
			XML_SetEndElementHandler(parser, NULL);
			XML_SetCharacterDataHandler(parser, NULL);
			parsed = XML_Parse(parser, "</root>", 7, 1);
			
			if (!parsed)
				throw Exception();
		}
	
	}
	catch (Exception& ex)
	{
		XML_ParserFree(parser);
		
		std::stringstream ss;
		if (ex.hasMessage())
		{
			ss << ex.getMessage();
		}
		else
		{
			ss << "Error while parsing XML: ";
			ss << XML_ErrorString(XML_GetErrorCode(parser));
		}
		ss << " at line ";
		ss << XML_GetCurrentLineNumber(parser);
		ss << " in file ";
		ss << fileName;
		throw Exception(ss.str());
	}
	
	XML_ParserFree(parser);
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


