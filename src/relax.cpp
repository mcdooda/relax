#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <lua5.2/lua.hpp>
#include "relax.h"
#include "xml.h"
#include "font.h"
#include "api/element.h"
#include "api/mouse.h"

namespace relax
{

lua_State* Relax::L;
std::map<std::string, std::set<Element*> > Relax::elementsByTagName;
bool Relax::justPressedKeys[SDLK_LAST];
bool Relax::justReleasedKeys[SDLK_LAST];
bool Relax::justPressedButtons[RELAX_NUM_BUTTONS];
bool Relax::justReleasedButtons[RELAX_NUM_BUTTONS];
Vector2 Relax::mouse;
Element* Relax::over;
Element* Relax::previousOver;

void Relax::init(lua_State* L1)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw Exception(SDL_GetError());
		
	memset(justPressedKeys, 0, sizeof(justPressedKeys));
	memset(justReleasedKeys, 0, sizeof(justReleasedKeys));
	
	L = L1;
	
	Element::init();
	Texture::init();
	Font::init();
	
	over = NULL;
	previousOver = NULL;
	
    api::element::open(L);
    api::mouse::open(L);
}

void Relax::quit()
{
	elementsByTagName.clear();
	
	Element::quit();
	Texture::quit();
	Font::quit();
	
	SDL_Quit();
}

lua_State* Relax::getLuaState()
{
	return L;
}

void Relax::setLuaState(lua_State* L1)
{
	L = L1;
}

Vector2 Relax::getDesktopResolution()
{
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	return Vector2(videoInfo->current_w, videoInfo->current_h);
}

void Relax::saveTag(Element* element)
{
	std::string tag = element->getTag();
	std::map<std::string, std::set<Element*> >::iterator it = elementsByTagName.find(tag);
	
	if (it != elementsByTagName.end())
	{
		it->second.insert(element);
	}
	else
	{
		elementsByTagName[tag] = std::set<Element*>();
		elementsByTagName[tag].insert(element);
	}
}

void Relax::unsaveTag(Element* element)
{
	std::string tag = element->getTag();
	std::map<std::string, std::set<Element*> >::iterator it = elementsByTagName.find(tag);
	
	if (it->second.size() == 1)
		elementsByTagName.erase(it);
		
	else
		it->second.erase(element);
}

std::set<Element*> Relax::getElementsByTagName(std::string tag)
{
	std::map<std::string, std::set<Element*> >::iterator it = elementsByTagName.find(tag);
	
	if (it != elementsByTagName.end())
		return it->second;
		
	else
		return std::set<Element*>();
}

Relax::Relax(Vector2 size, bool fullScreen, bool resizable) :
	Element("relax"),
	m_videoFlags(SDL_OPENGL),
	m_open(true)
{
	saveTag(this);
	
	if (fullScreen)
		m_videoFlags |= SDL_FULLSCREEN;
		
	if (resizable)
		m_videoFlags |= SDL_RESIZABLE;
	
	updateSize(size);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

bool Relax::isMouseOver()
{
	return true;
}

void Relax::pumpEvents()
{
	memset(justPressedKeys, 0, sizeof(justPressedKeys));
	memset(justReleasedKeys, 0, sizeof(justReleasedKeys));
	memset(justPressedButtons, 0, sizeof(justPressedButtons));
	memset(justReleasedButtons, 0, sizeof(justReleasedButtons));
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_KEYDOWN:
			justPressedKeys[event.key.keysym.sym] = true;
			break;
			
			case SDL_KEYUP:
			justReleasedKeys[event.key.keysym.sym] = true;
			break;
			
			case SDL_VIDEORESIZE:
			updateSize(Vector2(event.resize.w, event.resize.h));
			update();
			break;
			
			case SDL_MOUSEMOTION:
			mouse.setX(event.motion.x);
			mouse.setY(event.motion.y);
			checkMouseOver();
			checkMouseMove();
			break;
			
			case SDL_MOUSEBUTTONDOWN:
			justPressedButtons[event.button.button] = true;
			checkMouseDown();
			break;
			
			case SDL_MOUSEBUTTONUP:
			justReleasedButtons[event.button.button] = true;
			checkMouseUp();
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
	int code = luaL_dofile(L, fileName);
	checkLuaError(L, code);
}

void Relax::updateSize(Vector2 newSize)
{
	m_size.setWidth(newSize.getX());
	m_size.setHeight(newSize.getY());
	
	m_rectangle.setLeft(0);
	m_rectangle.setRight(newSize.getX());
	m_rectangle.setTop(0);
	m_rectangle.setBottom(newSize.getY());
	
	SDL_SetVideoMode(newSize.getX(), newSize.getY(), 32, m_videoFlags);
	glViewport(0, 0, newSize.getX(), newSize.getY());
	glLoadIdentity();
	gluOrtho2D(0, newSize.getX(), newSize.getY(), 0);
}

void Relax::checkMouseDown()
{
	if (over != NULL)
		over->handleMouseDown();
}

void Relax::checkMouseUp()
{
	if (over != NULL)
		over->handleMouseUp();
}

void Relax::checkMouseOver()
{
	previousOver = over;
	over = this;
	Element::checkMouseOver();
	if (previousOver != NULL && previousOver != over)
	{
		previousOver->handleMouseOut();
	}
	if (over != NULL && over != previousOver)
	{
		over->handleMouseOver();
	}
}

void Relax::checkMouseMove()
{
	if (over != NULL)
		over->handleMouseMove();
}

}


