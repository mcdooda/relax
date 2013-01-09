#include <stack>
#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <lua5.2/lua.hpp>
#include <expat.h>
#include "relax.h"

namespace relax
{

static Uint8 justPressedKeys[SDLK_LAST];
static Uint8 justReleasedKeys[SDLK_LAST];

void Relax::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw Exception(SDL_GetError());
		
	memset(justPressedKeys, 0, sizeof(justPressedKeys));
	memset(justReleasedKeys, 0, sizeof(justReleasedKeys));
	Element::init();
}

void Relax::quit()
{
	SDL_Quit();
}

Relax::Relax(bool fullScreen) :
	Element("relax"),
	m_open(true),
	m_fullScreen(fullScreen)
{
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	Uint32 flags = SDL_OPENGL;
	
	if (fullScreen)
		flags |= SDL_FULLSCREEN;
	
	m_size.setWidth(videoInfo->current_w);
	m_size.setHeight(videoInfo->current_h);
	SDL_SetVideoMode(m_size.getWidth(), m_size.getHeight(), videoInfo->vfmt->BitsPerPixel, flags);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glViewport(0, 0, m_size.getWidth(), m_size.getHeight());
	gluOrtho2D(0, m_size.getWidth(), m_size.getHeight(), 0);
	
	m_L = luaL_newstate();
}

Relax::~Relax()
{
	
}

void Relax::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	renderChildren();
	glFlush();
	SDL_GL_SwapBuffers();
}

void Relax::pumpEvents()
{
	memset(justPressedKeys, 0, sizeof(justPressedKeys));
	memset(justReleasedKeys, 0, sizeof(justReleasedKeys));
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
			justPressedKeys[event.key.keysym.sym] = 1;
			break;
			
			case SDL_KEYUP:
			justReleasedKeys[event.key.keysym.sym] = 1;
			break;
		}
	}
}

bool Relax::isPressed(Key key)
{
	Uint8* keys = SDL_GetKeyState(NULL);
	return keys[key];
}

bool Relax::isJustPressed(Key key)
{
	return justPressedKeys[key];
}

bool Relax::isJustReleased(Key key)
{
	return justReleasedKeys[key];
}

void Relax::runScript(const char* fileName)
{
	luaL_dofile(m_L, fileName);
}

struct XmlUserData
{
	std::stack<Element*> stack;
	Element* window;
};

/* expat callbacks */
static void startElement(void* userData, const char* name, const char** atts)
{
	XmlUserData* xud = (XmlUserData*) userData;
	std::stack<Element*>* stack = &xud->stack;
	
	Element* element = new Element(name, xud->window);
	
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
	XmlUserData* xud = (XmlUserData*) userData;
	std::stack<Element*>* stack = &xud->stack;
	
	if (stack->size() > 1)
		stack->pop();
}

static void characterData(void* userData, const char* s, int len)
{
	//std::stack<Element*>* stack = (std::stack<Element*>*) userData;
}

Element* Relax::elementFromXML(const char* xml)
{
	XML_Parser parser = XML_ParserCreate(NULL);
	XmlUserData xud;
	xud.window = this;
	XML_SetUserData(parser, &xud);
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
	return xud.stack.top();
}

void Relax::saveTag(Element* element)
{
	std::string tag = element->getTag();
	std::map<std::string, std::set<Element*> >::iterator it = m_elementsByTag.find(tag);
	
	if (it != m_elementsByTag.end())
	{
		it->second.insert(element);
	}
	else
	{
		m_elementsByTag[tag] = std::set<Element*>();
		m_elementsByTag[tag].insert(element);
	}
}

void Relax::unsaveTag(Element* element)
{
	std::string tag = element->getTag();
	std::map<std::string, std::set<Element*> >::iterator it = m_elementsByTag.find(tag);
	
	if (it->second.size() == 1)
		m_elementsByTag.erase(it);
		
	else
		it->second.erase(element);
}

std::set<Element*> Relax::getElementsByTag(std::string tag)
{
	std::map<std::string, std::set<Element*> >::iterator it = m_elementsByTag.find(tag);
	
	if (it != m_elementsByTag.end())
		return it->second;
		
	else
		return std::set<Element*>();
}

}


