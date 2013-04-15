#include <sstream>
#include <GL/gl.h>
#include "relax.h"
#include "element.h"
#include "exception.h"
#include "xml.h"
#include "api/element.h"

namespace relax
{

std::map<std::string, Element::AttrSetter> attrSetters;

Element::Element(std::string tag) :
	m_tag(tag),
	m_parent(NULL),
	m_anchor(TOP | LEFT),
	m_background(NULL),
	m_onMouseDown(LUA_NOREF),
	m_onMouseUp(LUA_NOREF),
	m_onMouseOver(LUA_NOREF),
	m_onMouseOut(LUA_NOREF),
	m_onMouseMove(LUA_NOREF)
{
	
}

Element::~Element()
{
	if (m_parent != NULL)
		m_parent->m_children.remove(this);
	
	lua_State* L = Relax::getLuaState();
	luaL_unref(L, LUA_REGISTRYINDEX, m_onMouseDown);
	luaL_unref(L, LUA_REGISTRYINDEX, m_onMouseOver);
	luaL_unref(L, LUA_REGISTRYINDEX, m_onMouseOut);
	
	Relax::unsaveTag(this);
}

void Element::addChild(Element* child)
{
	child->m_parent = this;
	m_children.push_back(child);
	saveChildTag(child);
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
		catch (Exception& ex)
		{
			if (ex.hasMessage())
				throw Exception(std::string("Error while handling value '" + attrValue + "' for attribute '" + attrName + "': ") + ex.getMessage() + " in <" + m_tag + ">");
				
			else
				throw Exception("Invalid value '" + attrValue + "' for attribute '" + attrName + "' in <" + m_tag + ">");
		}
	}
	else
		throw Exception("Unknown attribute '" + attrName + "' in <" + m_tag + ">");
}

void Element::addXML(const char* xml)
{
	xml::addXML(this, xml);
}

void Element::addXMLFile(const char* fileName)
{
	xml::addXMLFile(this, fileName);
}

void Element::render()
{
	draw();
	renderChildren();
}

void Element::checkMouseOver()
{
	if (isMouseOver())
	{
		Relax::setOverElement(this);
		for (std::list<Element*>::iterator it = m_children.begin(); it != m_children.end(); it++)
			(*it)->checkMouseOver();
	}
}

bool Element::isMouseOver()
{
	return m_rectangle.contains(Relax::getMouse());
}

void Element::handleMouseDown()
{
	handleEvent(m_onMouseDown);
}

void Element::handleMouseUp()
{
	handleEvent(m_onMouseUp);
}

void Element::handleMouseOver()
{
	handleEvent(m_onMouseOver);
}

void Element::handleMouseOut()
{
	handleEvent(m_onMouseOut);
}

void Element::handleMouseMove()
{
	handleEvent(m_onMouseMove);
}

void Element::saveChildTag(Element* child)
{
	Relax::saveTag(child);
	for (std::list<Element*>::iterator it = child->m_children.begin(); it != child->m_children.end(); it++)
		child->saveChildTag(*it);
}

void Element::renderChildren()
{
	for (std::list<Element*>::iterator it = m_children.begin(); it != m_children.end(); it++)
		(*it)->render();
}

void Element::draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (m_background != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		m_background->getImage()->bind();
		glTexCoordPointer(2, GL_FLOAT, 0, m_background->getTextureCoords());
	}
	
	m_color.use();
	glVertexPointer(2, GL_FLOAT, 0, m_vertices);
	glDrawArrays(GL_QUADS, 0, 4);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	
	if (m_background != NULL)
	{
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}

void Element::updatePosition()
{
	if (m_parent != NULL)
	{
		Vector2 computedSize;
		
		// x size
		if (m_size.getWidthAuto())
			computedSize.setX(m_parent->getComputedWidth() - m_parent->m_padding.getLeft() - m_parent->m_padding.getRight());
			
		else
			computedSize.setX(m_size.getWidth());
		
		// y size
		if (m_size.getHeightAuto())
			computedSize.setY(m_parent->getComputedHeight() - m_parent->m_padding.getTop() - m_parent->m_padding.getBottom());
			
		else
			computedSize.setY(m_size.getHeight());
			
		// x position
		if ((m_anchor & LEFT) == LEFT)
			m_rectangle.setLeft(m_parent->m_rectangle.getLeft() + m_relativePosition.getX() + m_parent->m_padding.getLeft());
			
		else if ((m_anchor & RIGHT) == RIGHT)
			m_rectangle.setLeft(m_parent->m_rectangle.getRight() + m_relativePosition.getX() - computedSize.getX() - m_parent->m_padding.getRight());
			
		else if ((m_anchor & CENTERX) == CENTERX)
			m_rectangle.setLeft(m_parent->m_rectangle.getCenterX() - m_parent->m_padding.getCenterX() + m_parent->m_padding.getLeft() + m_relativePosition.getX() - computedSize.getX() / 2);
			
		// y position
		if ((m_anchor & TOP) == TOP)
			m_rectangle.setTop(m_parent->m_rectangle.getTop() + m_relativePosition.getY() + m_parent->m_padding.getTop());
			
		else if ((m_anchor & BOTTOM) == BOTTOM)
			m_rectangle.setTop(m_parent->m_rectangle.getBottom() + m_relativePosition.getY() - computedSize.getY() - m_parent->m_padding.getBottom());
			
		else if ((m_anchor & CENTERY) == CENTERY)
			m_rectangle.setTop(m_parent->m_rectangle.getCenterY() - m_parent->m_padding.getCenterY() + m_parent->m_padding.getTop() + m_relativePosition.getY() - computedSize.getY() / 2);
			
		m_rectangle.setRight(m_rectangle.getLeft() + computedSize.getX());
		m_rectangle.setBottom(m_rectangle.getTop() + computedSize.getY());
	}
	else
	{
		if (!m_size.getWidthAuto())
		{
			m_rectangle.setLeft(0);
			m_rectangle.setRight(m_size.getWidth());
		}
		if (!m_size.getHeightAuto())
		{
			m_rectangle.setTop(0);
			m_rectangle.setBottom(m_size.getHeight());
		}
	}
	
	m_rectangle.copyToVertices(m_vertices);
	
	if (m_background != NULL)
		m_background->update(m_rectangle);

	for (std::list<Element*>::iterator it = m_children.begin(); it != m_children.end(); it++)
		(*it)->updatePosition();
}

void Element::init()
{
	attrSetters["anchor"] = &Element::setAttrAnchor;
	attrSetters["anchor-x"] = &Element::setAttrAnchorX;
	attrSetters["anchor-y"] = &Element::setAttrAnchorY;
	
	attrSetters["size"] = &Element::setAttrSize;
	attrSetters["size-x"] = &Element::setAttrSizeX;
	attrSetters["size-y"] = &Element::setAttrSizeY;
	
	attrSetters["position"] = &Element::setAttrPosition;
	attrSetters["position-x"] = &Element::setAttrPositionX;
	attrSetters["position-y"] = &Element::setAttrPositionY;
	
	attrSetters["color"] = &Element::setAttrColor;
	attrSetters["color-red"] = &Element::setAttrColorRed;
	attrSetters["color-green"] = &Element::setAttrColorGreen;
	attrSetters["color-blue"] = &Element::setAttrColorBlue;
	attrSetters["color-alpha"] = &Element::setAttrColorAlpha;
	
	attrSetters["padding"] = &Element::setAttrPadding;
	attrSetters["padding-left"] = &Element::setAttrPaddingLeft;
	attrSetters["padding-right"] = &Element::setAttrPaddingRight;
	attrSetters["padding-top"] = &Element::setAttrPaddingTop;
	attrSetters["padding-bottom"] = &Element::setAttrPaddingBottom;
	
	attrSetters["background"] = &Element::setAttrBackground;
	attrSetters["background-image"] = &Element::setAttrBackgroundImage;
	attrSetters["background-repeat"] = &Element::setAttrBackgroundRepeat;
	
	attrSetters["onmousedown"] = &Element::setAttrOnMouseDown;
	attrSetters["onmouseup"] = &Element::setAttrOnMouseUp;
	attrSetters["onmouseover"] = &Element::setAttrOnMouseOver;
	attrSetters["onmouseout"] = &Element::setAttrOnMouseOut;
	attrSetters["onmousemove"] = &Element::setAttrOnMouseMove;
}

void Element::quit()
{
	
}

void Element::checkLuaError(lua_State* L, int code)
{
	if (code != LUA_OK)
	{
		switch (code)
		{
			case LUA_ERRRUN:
			std::cerr << "runtime error:" << std::endl;
			break;
			
			case LUA_ERRMEM:
			std::cerr << "memory allocation error:" << std::endl;
			break;
			
			case LUA_ERRGCMM:
			std::cerr << "error while running a __gc metamethod:" << std::endl;
			break;
		}
		std::cerr << lua_tostring(L, -1) << std::endl;
		lua_pop(L, 1);
	}
}

void Element::handleEvent(int handler)
{
	if (handler != LUA_NOREF)
	{
		lua_State* L = Relax::getLuaState();
		lua_pushinteger(L, handler);
		lua_rawget(L, LUA_REGISTRYINDEX);
		api::element::newRef(L, this);
		//lua_pushlightuserdata(L, this);
		int code = lua_pcall(L, 1, 0, 0);
		checkLuaError(L, code);
	}
}

void Element::setAttrAnchor(std::string attrValue)
{
	std::string anchorX, anchorY;
	std::istringstream ss(attrValue);
	ss >> anchorX;
	ss >> anchorY;
	setAttrAnchorX(anchorX);
	setAttrAnchorY(anchorY);
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

void Element::setAttrSize(std::string attrValue)
{
	std::string width, height;
	std::istringstream ss(attrValue);
	ss >> width;
	ss >> height;
	setAttrSizeX(width);
	setAttrSizeY(height);
}

void Element::setAttrSizeX(std::string attrValue)
{
	if (attrValue == "auto")
		setWidthAuto();
	
	else
	{
		float width;
		std::istringstream ss(attrValue);
		ss >> width;
		if (ss.fail())
			throw Exception();
			
		setWidth(width);
	}
}

void Element::setAttrSizeY(std::string attrValue)
{
	if (attrValue == "auto")
		setHeightAuto();
	
	else
	{
		float height;
		std::istringstream ss(attrValue);
		ss >> height;
		if (ss.fail())
			throw Exception();
			
		setHeight(height);
	}
}

void Element::setAttrPosition(std::string attrValue)
{
	float x, y;
	std::istringstream ss(attrValue);
	ss >> x;
	ss >> y;
	if (ss.fail())
		throw Exception();
		
	setPosition(Vector2(x, y));
}

void Element::setAttrPositionX(std::string attrValue)
{
	float x;
	std::istringstream ss(attrValue);
	ss >> x;
	if (ss.fail())
		throw Exception();
		
	setX(x);
}

void Element::setAttrPositionY(std::string attrValue)
{
	float y;
	std::istringstream ss(attrValue);
	ss >> y;
	if (ss.fail())
		throw Exception();
		
	setY(y);
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
		
	setColor(Color(r, g, b, a));
}

void Element::setAttrColorRed(std::string attrValue)
{
	unsigned int r;
	std::istringstream ss(attrValue);
	ss >> r;
	if (ss.fail())
		throw Exception();
		
	setRed(r);
}

void Element::setAttrColorGreen(std::string attrValue)
{
	unsigned int g;
	std::istringstream ss(attrValue);
	ss >> g;
	if (ss.fail())
		throw Exception();
		
	setGreen(g);
}

void Element::setAttrColorBlue(std::string attrValue)
{
	unsigned int b;
	std::istringstream ss(attrValue);
	ss >> b;
	if (ss.fail())
		throw Exception();
		
	setBlue(b);
}

void Element::setAttrColorAlpha(std::string attrValue)
{
	unsigned int a;
	std::istringstream ss(attrValue);
	ss >> a;
	if (ss.fail())
		throw Exception();
		
	setAlpha(a);
}

void Element::setAttrPadding(std::string attrValue)
{
	float paddingTop, paddingLeft, paddingRight, paddingBottom;
	std::istringstream ss(attrValue);
	ss >> paddingTop;
	ss >> paddingLeft;
	ss >> paddingRight;
	ss >> paddingBottom;
	if (ss.fail())
		throw Exception();
		
	setPadding(Padding(paddingTop, paddingLeft, paddingRight, paddingBottom));
}

void Element::setAttrPaddingLeft(std::string attrValue)
{
	float paddingLeft;
	std::istringstream ss(attrValue);
	ss >> paddingLeft;
	if (ss.fail())
		throw Exception();
		
	setPaddingLeft(paddingLeft);
}

void Element::setAttrPaddingRight(std::string attrValue)
{
	float paddingRight;
	std::istringstream ss(attrValue);
	ss >> paddingRight;
	if (ss.fail())
		throw Exception();
		
	setPaddingRight(paddingRight);
}

void Element::setAttrPaddingTop(std::string attrValue)
{
	float paddingTop;
	std::istringstream ss(attrValue);
	ss >> paddingTop;
	if (ss.fail())
		throw Exception();
		
	setPaddingTop(paddingTop);
}

void Element::setAttrPaddingBottom(std::string attrValue)
{
	float paddingBottom;
	std::istringstream ss(attrValue);
	ss >> paddingBottom;
	if (ss.fail())
		throw Exception();
		
	setPaddingBottom(paddingBottom);
}

void Element::setAttrBackground(std::string attrValue)
{
	std::string backgroundImage, backgroundRepeat;
	std::istringstream ss(attrValue);
	ss >> backgroundImage;
	ss >> backgroundRepeat;
	if (ss.fail())
		throw Exception();
		
	setAttrBackgroundImage(backgroundImage);
	setAttrBackgroundRepeat(backgroundRepeat);
}

void Element::setAttrBackgroundImage(std::string attrValue)
{
	if (m_background == NULL)
		setBackground(new Background());
		
	setBackgroundImage(Texture::get(attrValue));
}

void Element::setAttrBackgroundRepeat(std::string attrValue)
{
	if (m_background == NULL)
		setBackground(new Background());
		
	Background::Repeat backgroundRepeat;
	
	if (attrValue == "scale")
		backgroundRepeat = Background::SCALE;
		
	else if (attrValue == "repeat")
		backgroundRepeat = Background::REPEAT;
		
	else
		throw Exception();
		
	setBackgroundRepeat(backgroundRepeat);
}

void Element::setAttrOnEvent(std::string attrValue, int* handler)
{
	lua_State* L = Relax::getLuaState();
	luaL_unref(L, LUA_REGISTRYINDEX, *handler);
	std::string fullCode = "local self = ...; " + attrValue;
	//std::cout << "compiling code: " << fullCode << std::endl;
	int code = luaL_loadstring(L, fullCode.c_str());
	checkLuaError(L, code);
	if (code == LUA_OK)
		*handler = luaL_ref(L, LUA_REGISTRYINDEX);
}

void Element::setAttrOnMouseDown(std::string attrValue)
{
	setAttrOnEvent(attrValue, &m_onMouseDown);
}

void Element::setAttrOnMouseUp(std::string attrValue)
{
	setAttrOnEvent(attrValue, &m_onMouseUp);
}

void Element::setAttrOnMouseOver(std::string attrValue)
{
	setAttrOnEvent(attrValue, &m_onMouseOver);
}

void Element::setAttrOnMouseOut(std::string attrValue)
{
	setAttrOnEvent(attrValue, &m_onMouseOut);
}

void Element::setAttrOnMouseMove(std::string attrValue)
{
	setAttrOnEvent(attrValue, &m_onMouseMove);
}

}


