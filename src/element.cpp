#include <iostream>
#include <stack>
#include <sstream>
#include <cstring>
#include <GL/gl.h>
#include <expat.h>
#include "element.h"
#include "exception.h"

namespace relax
{

std::map<std::string, Element::AttrSetter> attrSetters;

Element::Element() :
	m_parent(NULL),
	m_anchor(TOP | LEFT)
{

}

Element::~Element()
{
	if (m_parent != NULL)
		m_parent->m_children.remove(this);
}

void Element::addChild(Element* child)
{
	child->m_parent = this;
	m_children.push_back(child);
}

void Element::setAttribute(std::string attrName, std::string attrValue)
{
	std::map<std::string, AttrSetter>::iterator it = attrSetters.find(attrName);
	
	if (it != attrSetters.end())
	{
		try
		{
			AttrSetter attrSetter = it->second;
			(this->*attrSetter)(attrValue);
		}
		catch (Exception ex)
		{
			throw Exception("Invalid value '" + attrValue + "' for attribute '" + attrName + "'");
		}
	}
	else
		throw Exception("Unknown attribute '" + attrName + "'");
}

void Element::render()
{
	draw();
	renderChildren();
}

void Element::renderChildren()
{
	for (std::list<Element*>::iterator it = m_children.begin(); it != m_children.end(); it++)
		(*it)->render();
}

void Element::draw()
{
	m_color.use();
	int left = m_absolutePosition.getX();
	int top = m_absolutePosition.getY();
	int right = left + m_realSize.getX();
	int bottom = top + m_realSize.getY();
	glBegin(GL_QUADS);
		glVertex2i(left, top);
		glVertex2i(right, top);
		glVertex2i(right, bottom);
		glVertex2i(left, bottom);
	glEnd();
}

void Element::updatePosition()
{
	if (m_parent != NULL)
	{
		// x size
		if (m_size.getWidthAuto())
			m_realSize.setX(m_parent->m_realSize.getX() - m_parent->m_padding.getLeft() - m_parent->m_padding.getRight());
			
		else
			m_realSize.setX(m_size.getWidth());
		
		// y size
		if (m_size.getHeightAuto())
			m_realSize.setY(m_parent->m_realSize.getY() - m_parent->m_padding.getTop() - m_parent->m_padding.getBottom());
			
		else
			m_realSize.setY(m_size.getHeight());
			
		// x position
		if ((m_anchor & LEFT) == LEFT)
			m_absolutePosition.setX(m_parent->m_absolutePosition.getX() + m_relativePosition.getX() + m_parent->m_padding.getLeft());
			
		else if ((m_anchor & RIGHT) == RIGHT)
			m_absolutePosition.setX(m_parent->m_absolutePosition.getX() + m_relativePosition.getX() + m_parent->m_realSize.getX() - m_realSize.getX() - m_parent->m_padding.getRight());
			
		else if ((m_anchor & CENTERX) == CENTERX)
			m_absolutePosition.setX(m_parent->m_absolutePosition.getX() + (m_parent->m_realSize.getX() - m_parent->m_padding.getLeft() - m_parent->m_padding.getRight()) / 2 + m_parent->m_padding.getLeft() + m_relativePosition.getX() - m_realSize.getX() / 2);
			
		// y position
		if ((m_anchor & TOP) == TOP)
			m_absolutePosition.setY(m_parent->m_absolutePosition.getY() + m_relativePosition.getY() + m_parent->m_padding.getTop());
			
		else if ((m_anchor & BOTTOM) == BOTTOM)
			m_absolutePosition.setY(m_parent->m_absolutePosition.getY() + m_relativePosition.getY() + m_parent->m_realSize.getY() - m_realSize.getY() - m_parent->m_padding.getBottom());
			
		else if ((m_anchor & CENTERY) == CENTERY)
			m_absolutePosition.setY(m_parent->m_absolutePosition.getY() + (m_parent->m_realSize.getY() - m_parent->m_padding.getTop() - m_parent->m_padding.getBottom()) / 2 + m_parent->m_padding.getTop() + m_relativePosition.getY() - m_realSize.getY() / 2);
	}
	else
	{
		if (!m_size.getWidthAuto())
			m_realSize.setX(m_size.getWidth());
			
		if (!m_size.getHeightAuto())
			m_realSize.setY(m_size.getHeight());
	}
	
	for (std::list<Element*>::iterator it = m_children.begin(); it != m_children.end(); it++)
		(*it)->updatePosition();
}

/* expat callbacks */
static void startElement(void* userData, const char* name, const char** atts)
{
	std::stack<Element*>* stack = (std::stack<Element*>*) userData;
	
	for (int i = 0; i < stack->size(); i++)
		std::cout << " ";
	std::cout << "+ " << name << std::endl;
	
	Element* element = new Element();
	
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
		
	for (int i = 0; i < stack->size(); i++)
		std::cout << " ";
	std::cout << "- " << name << std::endl;
}

static void characterData(void* userData, const char* s, int len)
{
	std::stack<Element*>* stack = (std::stack<Element*>*) userData;
}

Element* Element::fromXML(const char* xml)
{
	XML_Parser parser = XML_ParserCreate(NULL);
	std::stack<Element*> stack;
	XML_SetUserData(parser, &stack);
	XML_SetStartElementHandler(parser, startElement);
	XML_SetEndElementHandler(parser, endElement);
	XML_SetCharacterDataHandler(parser, characterData);
	int parsed = XML_Parse(parser, xml, strlen(xml), 1);
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
	return stack.top();
}

void Element::init()
{
	attrSetters["anchorx"] = &Element::setAttrAnchorX;
	attrSetters["anchory"] = &Element::setAttrAnchorY;
	attrSetters["width"] = &Element::setAttrWidth;
	attrSetters["height"] = &Element::setAttrHeight;
	attrSetters["x"] = &Element::setAttrX;
	attrSetters["y"] = &Element::setAttrY;
	attrSetters["color"] = &Element::setAttrColor;
	attrSetters["padding"] = &Element::setAttrPadding;
	attrSetters["padding-left"] = &Element::setAttrPaddingLeft;
	attrSetters["padding-right"] = &Element::setAttrPaddingRight;
	attrSetters["padding-top"] = &Element::setAttrPaddingTop;
	attrSetters["padding-bottom"] = &Element::setAttrPaddingBottom;
}

void Element::setAttrAnchorX(std::string attrValue)
{
	Anchor anchor;
	if (attrValue == "left")
		anchor = LEFT;
	
	else if (attrValue == "center")
		anchor = CENTERX;
	
	else if (attrValue == "right")
		anchor = RIGHT;
	
	else
		throw Exception();
		
	setAnchorX(anchor);
}

void Element::setAttrAnchorY(std::string attrValue)
{
	Anchor anchor;
	if (attrValue == "top")
		anchor = TOP;
	
	else if (attrValue == "center")
		anchor = CENTERY;
	
	else if (attrValue == "bottom")
		anchor = BOTTOM;
	
	else
		throw Exception();
		
	setAnchorY(anchor);
}

void Element::setAttrWidth(std::string attrValue)
{
	if (attrValue == "auto")
		m_size.setWidthAuto();
	
	else
	{
		int width;
		std::istringstream ss(attrValue);
		ss >> width;
		if (ss.fail())
			throw Exception();
			
		m_size.setWidth(width);
	}
}

void Element::setAttrHeight(std::string attrValue)
{
	if (attrValue == "auto")
		m_size.setHeightAuto();
	
	else
	{
		int height;
		std::istringstream ss(attrValue);
		ss >> height;
		if (ss.fail())
			throw Exception();
			
		m_size.setHeight(height);
	}
}

void Element::setAttrX(std::string attrValue)
{
	int x;
	std::istringstream ss(attrValue);
	ss >> x;
	if (ss.fail())
		throw Exception();
		
	m_relativePosition.setX(x);
}

void Element::setAttrY(std::string attrValue)
{
	int y;
	std::istringstream ss(attrValue);
	ss >> y;
	if (ss.fail())
		throw Exception();
		
	m_relativePosition.setY(y);
}

void Element::setAttrColor(std::string attrValue)
{
	unsigned int r, g, b, a;
	std::istringstream ss(attrValue);
	ss >> r;
	ss >> g;
	ss >> b;
	ss >> a;
	if (ss.fail())
		throw Exception();
		
	m_color = Color(r, g, b, a);
}

void Element::setAttrPadding(std::string attrValue)
{
	int padding;
	std::istringstream ss(attrValue);
	ss >> padding;
	if (ss.fail())
		throw Exception();
		
	m_padding.setAll(padding);
}

void Element::setAttrPaddingLeft(std::string attrValue)
{
	int paddingLeft;
	std::istringstream ss(attrValue);
	ss >> paddingLeft;
	if (ss.fail())
		throw Exception();
		
	m_padding.setLeft(paddingLeft);
}

void Element::setAttrPaddingRight(std::string attrValue)
{
	int paddingRight;
	std::istringstream ss(attrValue);
	ss >> paddingRight;
	if (ss.fail())
		throw Exception();
		
	m_padding.setRight(paddingRight);
}

void Element::setAttrPaddingTop(std::string attrValue)
{
	int paddingTop;
	std::istringstream ss(attrValue);
	ss >> paddingTop;
	if (ss.fail())
		throw Exception();
		
	m_padding.setTop(paddingTop);
}

void Element::setAttrPaddingBottom(std::string attrValue)
{
	int paddingBottom;
	std::istringstream ss(attrValue);
	ss >> paddingBottom;
	if (ss.fail())
		throw Exception();
		
	m_padding.setAll(paddingBottom);
}

}

