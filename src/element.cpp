#include <sstream>
#include <GL/gl.h>
#include "relax.h"
#include "element.h"
#include "text.h"
#include "exception.h"
#include "xml.h"
#include "api/element.h"
#include "attrsetter.h"
#include "luadebug.h"

namespace relax
{

static std::map<std::string, Element::AttrParser> attrParsers;

static int cacheTable;

Element::Element(std::string tag) :
	m_tag(tag),
	m_parent(NULL),
	m_anchor(TOP | LEFT),
	m_background(NULL),
	m_font(Font::getDefault()),
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
	luaL_unref(L, LUA_REGISTRYINDEX, m_onMouseUp);
	luaL_unref(L, LUA_REGISTRYINDEX, m_onMouseOver);
	luaL_unref(L, LUA_REGISTRYINDEX, m_onMouseOut);
	luaL_unref(L, LUA_REGISTRYINDEX, m_onMouseMove);
	
	Relax::unsaveTag(this);
}

void Element::addChild(Element* child)
{
	child->m_parent = this;
	m_children.push_back(child);
	saveChildTag(child);
}

void Element::setBackground(Background* background)
{
	delete m_background;
	m_background = background;
}

void Element::setBackgroundImage(Texture* backgroundImage)
{
	if (m_background == NULL)
		m_background = new Background();
		
	m_background->setImage(backgroundImage);
}

void Element::setBackgroundRepeat(BackgroundRepeat backgroundRepeat)
{
	if (m_background == NULL)
		m_background = new Background();
		
	m_background->setRepeat(backgroundRepeat);
}

bool Element::isText()
{
	return m_background != NULL && m_background->getImage()->isText();
}

bool Element::containsText()
{
	return m_children.size() == 1 && getTextElement()->isText();
}

std::string Element::getText()
{
	if (isText())
		return ((Text*) m_background->getImage())->getString();
		
	else
		return "";
}

void Element::setAttribute(std::string attrName, std::string attrValue)
{
	try
	{
		AttrSetter* attrSetter = getAttrSetter(attrName, attrValue);
		attrSetter->set(this);
		delete attrSetter;
	}
	catch (Exception& ex)
	{
		throw Exception(ex.getMessage() + " in tag <" + m_tag + ">");
	}
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

AttrSetter* Element::getAttrSetter(std::string attrName, std::string attrValue)
{
	std::map<std::string, Element::AttrParser>::iterator it = attrParsers.find(attrName);
	
	if (it != attrParsers.end())
	{
		try
		{
			AttrParser attrParser = it->second;
			AttrSetter* attrSetter = (*attrParser)(attrValue);
			return attrSetter;
		}
		catch (Exception& ex)
		{
			if (ex.hasMessage())
				throw Exception(std::string("Error while handling value '" + attrValue + "' for attribute '" + attrName + "': ") + ex.getMessage());
				
			else
				throw Exception("Invalid value '" + attrValue + "' for attribute '" + attrName + "'");
		}
	}
	else
		throw Exception("Unknown attribute '" + attrName + "'");
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
		m_background->update(this);

	for (std::list<Element*>::iterator it = m_children.begin(); it != m_children.end(); it++)
		(*it)->updatePosition();
}

void Element::setEventHandler(int* handler, int ref)
{
	lua_State* L = Relax::getLuaState();
	luaL_unref(L, LUA_REGISTRYINDEX, *handler);
	lua_pushinteger(L, cacheTable);
	lua_rawget(L, LUA_REGISTRYINDEX);
	lua_pushinteger(L, ref);
	lua_rawget(L, -2);
	*handler = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_pop(L, 1);
}

void Element::handleEvent(int handler)
{
	if (handler != LUA_NOREF)
	{
		lua_State* L = Relax::getLuaState();
		lua_pushinteger(L, handler);
		lua_rawget(L, LUA_REGISTRYINDEX);
		api::element::newRef(L, this);
		int code = lua_pcall(L, 1, 0, 0);
		checkLuaError(L, code);
	}
}

void Element::init()
{
	attrParsers["anchor"] = &Element::setAttrAnchor;
	attrParsers["anchor-x"] = &Element::setAttrAnchorX;
	attrParsers["anchor-y"] = &Element::setAttrAnchorY;
	
	attrParsers["size"] = &Element::setAttrSize;
	attrParsers["size-x"] = &Element::setAttrSizeX;
	attrParsers["size-y"] = &Element::setAttrSizeY;
	
	attrParsers["position"] = &Element::setAttrPosition;
	attrParsers["position-x"] = &Element::setAttrPositionX;
	attrParsers["position-y"] = &Element::setAttrPositionY;
	
	attrParsers["color"] = &Element::setAttrColor;
	attrParsers["color-red"] = &Element::setAttrColorRed;
	attrParsers["color-green"] = &Element::setAttrColorGreen;
	attrParsers["color-blue"] = &Element::setAttrColorBlue;
	attrParsers["color-alpha"] = &Element::setAttrColorAlpha;
	
	attrParsers["padding"] = &Element::setAttrPadding;
	attrParsers["padding-left"] = &Element::setAttrPaddingLeft;
	attrParsers["padding-right"] = &Element::setAttrPaddingRight;
	attrParsers["padding-top"] = &Element::setAttrPaddingTop;
	attrParsers["padding-bottom"] = &Element::setAttrPaddingBottom;
	
	attrParsers["background"] = &Element::setAttrBackground;
	attrParsers["background-image"] = &Element::setAttrBackgroundImage;
	attrParsers["background-repeat"] = &Element::setAttrBackgroundRepeat;
	
	attrParsers["onmousedown"] = &Element::setAttrOnMouseDown;
	attrParsers["onmouseup"] = &Element::setAttrOnMouseUp;
	attrParsers["onmouseover"] = &Element::setAttrOnMouseOver;
	attrParsers["onmouseout"] = &Element::setAttrOnMouseOut;
	attrParsers["onmousemove"] = &Element::setAttrOnMouseMove;
	
	lua_State* L = Relax::getLuaState();
	lua_newtable(L);
	cacheTable = luaL_ref(L, LUA_REGISTRYINDEX);
	lua_newtable(L);
	lua_pushstring(L, "v");
	lua_setfield(L, -2, "__mode");
	lua_setmetatable(L, -2);
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

AttrSetter* Element::setAttrAnchor(std::string attrValue)
{
	std::string anchorX, anchorY;
	std::istringstream ss(attrValue);
	ss >> anchorX;
	ss >> anchorY;
	AttrSetterList<2>* attrSetter = new AttrSetterList<2>();
	attrSetter->add(0, setAttrAnchorX(anchorX));
	attrSetter->add(1, setAttrAnchorY(anchorY));
	return attrSetter;
}

AttrSetter* Element::setAttrAnchorX(std::string attrValue)
{
	Anchor anchorX;
	if (attrValue == "left")
		anchorX = LEFT;
	
	else if (attrValue == "center")
		anchorX = CENTERX;
	
	else if (attrValue == "right")
		anchorX = RIGHT;
	
	else
		throw Exception();
		
	return new AttrSetterAnchorX(anchorX);
}

AttrSetter* Element::setAttrAnchorY(std::string attrValue)
{
	Anchor anchorY;
	if (attrValue == "top")
		anchorY = TOP;
	
	else if (attrValue == "center")
		anchorY = CENTERY;
	
	else if (attrValue == "bottom")
		anchorY = BOTTOM;
	
	else
		throw Exception();
		
	return new AttrSetterAnchorY(anchorY);
}

AttrSetter* Element::setAttrSize(std::string attrValue)
{
	std::string width, height;
	std::istringstream ss(attrValue);
	ss >> width;
	ss >> height;
	AttrSetterList<2>* attrSetter = new AttrSetterList<2>();
	attrSetter->add(0, setAttrSizeX(width));
	attrSetter->add(1, setAttrSizeY(height));
	return attrSetter;
}

AttrSetter* Element::setAttrSizeX(std::string attrValue)
{
	if (attrValue == "auto")
		return new AttrSetterSizeXAuto();
	
	else
	{
		float width;
		std::istringstream ss(attrValue);
		ss >> width;
		if (ss.fail())
			throw Exception();
			
		return new AttrSetterSizeX(width);
	}
}

AttrSetter* Element::setAttrSizeY(std::string attrValue)
{
	if (attrValue == "auto")
		return new AttrSetterSizeYAuto();
	
	else
	{
		float height;
		std::istringstream ss(attrValue);
		ss >> height;
		if (ss.fail())
			throw Exception();
			
		return new AttrSetterSizeY(height);
	}
}

AttrSetter* Element::setAttrPosition(std::string attrValue)
{
	float x, y;
	std::istringstream ss(attrValue);
	ss >> x;
	ss >> y;
	if (ss.fail())
		throw Exception();
		
	AttrSetterList<2>* attrSetter = new AttrSetterList<2>();
	attrSetter->add(0, new AttrSetterPositionX(x));
	attrSetter->add(1, new AttrSetterPositionY(y));
	return attrSetter;
}

AttrSetter* Element::setAttrPositionX(std::string attrValue)
{
	float x;
	std::istringstream ss(attrValue);
	ss >> x;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterPositionX(x);
}

AttrSetter* Element::setAttrPositionY(std::string attrValue)
{
	float y;
	std::istringstream ss(attrValue);
	ss >> y;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterPositionY(y);
}

AttrSetter* Element::setAttrColor(std::string attrValue)
{
	unsigned int r, g, b, a;
	std::istringstream ss(attrValue);
	ss >> r;
	ss >> g;
	ss >> b;
	ss >> a;
	if (ss.fail())
		throw Exception();
		
	AttrSetterList<4>* attrSetter = new AttrSetterList<4>();
	attrSetter->add(0, new AttrSetterColorRed(r));
	attrSetter->add(1, new AttrSetterColorGreen(g));
	attrSetter->add(2, new AttrSetterColorBlue(b));
	attrSetter->add(3, new AttrSetterColorAlpha(a));
	return attrSetter;
}

AttrSetter* Element::setAttrColorRed(std::string attrValue)
{
	unsigned int r;
	std::istringstream ss(attrValue);
	ss >> r;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterColorRed(r);
}

AttrSetter* Element::setAttrColorGreen(std::string attrValue)
{
	unsigned int g;
	std::istringstream ss(attrValue);
	ss >> g;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterColorGreen(g);
}

AttrSetter* Element::setAttrColorBlue(std::string attrValue)
{
	unsigned int b;
	std::istringstream ss(attrValue);
	ss >> b;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterColorBlue(b);
}

AttrSetter* Element::setAttrColorAlpha(std::string attrValue)
{
	unsigned int a;
	std::istringstream ss(attrValue);
	ss >> a;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterColorAlpha(a);
}

AttrSetter* Element::setAttrPadding(std::string attrValue)
{
	float paddingTop, paddingLeft, paddingRight, paddingBottom;
	std::istringstream ss(attrValue);
	ss >> paddingTop;
	ss >> paddingLeft;
	ss >> paddingRight;
	ss >> paddingBottom;
	if (ss.fail())
		throw Exception();
		
	AttrSetterList<4>* attrSetter = new AttrSetterList<4>();
	attrSetter->add(0, new AttrSetterPaddingTop(paddingTop));
	attrSetter->add(1, new AttrSetterPaddingLeft(paddingLeft));
	attrSetter->add(2, new AttrSetterPaddingRight(paddingRight));
	attrSetter->add(3, new AttrSetterPaddingBottom(paddingBottom));
	return attrSetter;
}

AttrSetter* Element::setAttrPaddingLeft(std::string attrValue)
{
	float paddingLeft;
	std::istringstream ss(attrValue);
	ss >> paddingLeft;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterPaddingLeft(paddingLeft);
}

AttrSetter* Element::setAttrPaddingRight(std::string attrValue)
{
	float paddingRight;
	std::istringstream ss(attrValue);
	ss >> paddingRight;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterPaddingRight(paddingRight);
}

AttrSetter* Element::setAttrPaddingTop(std::string attrValue)
{
	float paddingTop;
	std::istringstream ss(attrValue);
	ss >> paddingTop;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterPaddingTop(paddingTop);
}

AttrSetter* Element::setAttrPaddingBottom(std::string attrValue)
{
	float paddingBottom;
	std::istringstream ss(attrValue);
	ss >> paddingBottom;
	if (ss.fail())
		throw Exception();
		
	return new AttrSetterPaddingBottom(paddingBottom);
}

AttrSetter* Element::setAttrBackground(std::string attrValue)
{
	std::string backgroundImage, backgroundRepeat;
	std::istringstream ss(attrValue);
	ss >> backgroundImage;
	ss >> backgroundRepeat;
	if (ss.fail())
		throw Exception();
		
	AttrSetterList<2>* attrSetter = new AttrSetterList<2>();
	attrSetter->add(0, setAttrBackgroundImage(backgroundImage));
	attrSetter->add(1, setAttrBackgroundRepeat(backgroundRepeat));
	return attrSetter;
}

AttrSetter* Element::setAttrBackgroundImage(std::string attrValue)
{
	return new AttrSetterBackgroundImage(Texture::get(attrValue));
}

AttrSetter* Element::setAttrBackgroundRepeat(std::string attrValue)
{
	BackgroundRepeat backgroundRepeat;
	
	if (attrValue == "scale")
		backgroundRepeat = SCALE;
		
	else if (attrValue == "repeat")
		backgroundRepeat = REPEAT;
		
	else
		throw Exception();
		
	return new AttrSetterBackgroundRepeat(backgroundRepeat);
}

int Element::parseEventHandler(std::string attrValue)
{
	lua_State* L = Relax::getLuaState();
	lua_pushinteger(L, cacheTable);
	lua_rawget(L, LUA_REGISTRYINDEX);
	std::string fullCode = "local self = ...; " + attrValue;
	int code = luaL_loadstring(L, fullCode.c_str());
	checkLuaError(L, code);
	if (code == LUA_OK)
	{
		int ref = luaL_ref(L, -2);
		lua_pop(L, 1);
		return ref;
	}
	else
		throw Exception();
}

AttrSetter* Element::setAttrOnMouseDown(std::string attrValue)
{
	return new AttrSetterOnMouseDown(parseEventHandler(attrValue));
}

AttrSetter* Element::setAttrOnMouseUp(std::string attrValue)
{
	return new AttrSetterOnMouseUp(parseEventHandler(attrValue));
}

AttrSetter* Element::setAttrOnMouseOver(std::string attrValue)
{
	return new AttrSetterOnMouseOver(parseEventHandler(attrValue));
}

AttrSetter* Element::setAttrOnMouseOut(std::string attrValue)
{
	return new AttrSetterOnMouseOut(parseEventHandler(attrValue));
}

AttrSetter* Element::setAttrOnMouseMove(std::string attrValue)
{
	return new AttrSetterOnMouseMove(parseEventHandler(attrValue));
}

}


