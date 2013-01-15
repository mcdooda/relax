#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <lua5.2/lua.hpp>
#include "relax.h"
#include "xml.h"

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

Relax::Relax(bool fullScreen, bool resizable) :
	Element("relax"),
	m_videoFlags(SDL_OPENGL),
	m_open(true)
{
	if (fullScreen)
		m_videoFlags |= SDL_FULLSCREEN;
		
	if (resizable)
		m_videoFlags |= SDL_RESIZABLE;
	
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	updateSize(videoInfo->current_w, videoInfo->current_h);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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
			
			case SDL_VIDEORESIZE:
			updateSize(event.resize.w, event.resize.h);
			update();
			break;
			
			case SDL_QUIT:
			m_open = false;
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

void Relax::updateSize(int width, int height)
{
	m_size.setWidth(width);
	m_size.setHeight(height);
	
	m_rectangle.setLeft(0);
	m_rectangle.setRight(width);
	m_rectangle.setTop(0);
	m_rectangle.setBottom(height);
	
	SDL_SetVideoMode(width, height, 32, m_videoFlags);
	glViewport(0, 0, width, height);
	glLoadIdentity();
	gluOrtho2D(0, width, height, 0);
}

}


