#include <sstream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <lua5.2/lua.hpp>
#include "relax.h"
#include "xml.h"
#include "font.h"

namespace relax
{

lua_State* Relax::L;
std::map<std::string, std::set<Element*> > Relax::elementsByTag;
bool Relax::justPressedKeys[SDLK_LAST];
bool Relax::justReleasedKeys[SDLK_LAST];
bool Relax::justPressedButtons[RELAX_NUM_BUTTONS];
bool Relax::justReleasedButtons[RELAX_NUM_BUTTONS];
Vector2 Relax::mouse;
Element* Relax::over;

void Relax::init(lua_State* L1)
{
	L = L1;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw Exception(SDL_GetError());
		
	memset(justPressedKeys, 0, sizeof(justPressedKeys));
	memset(justReleasedKeys, 0, sizeof(justReleasedKeys));
	
	Element::init();
	Texture::init();
	Font::init();
	
	over = NULL;
}

void Relax::quit()
{
	elementsByTag.clear();
	
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
	std::map<std::string, std::set<Element*> >::iterator it = elementsByTag.find(tag);
	
	if (it != elementsByTag.end())
	{
		it->second.insert(element);
	}
	else
	{
		elementsByTag[tag] = std::set<Element*>();
		elementsByTag[tag].insert(element);
	}
}

void Relax::unsaveTag(Element* element)
{
	std::string tag = element->getTag();
	std::map<std::string, std::set<Element*> >::iterator it = elementsByTag.find(tag);
	
	if (it->second.size() == 1)
		elementsByTag.erase(it);
		
	else
		it->second.erase(element);
}

std::set<Element*> Relax::getElementsByTag(std::string tag)
{
	std::map<std::string, std::set<Element*> >::iterator it = elementsByTag.find(tag);
	
	if (it != elementsByTag.end())
		return it->second;
		
	else
		return std::set<Element*>();
}

void Relax::setOverElement(Element* element)
{
	over = element;
}

Vector2 Relax::getMouse()
{
	return mouse;
}

Relax::Relax(Vector2 size, bool fullScreen, bool resizable) :
	Element("relax"),
	m_videoFlags(SDL_OPENGL),
	m_open(true)
{
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
			break;
			
			case SDL_MOUSEBUTTONDOWN:
			justPressedButtons[event.button.button] = true;
			checkClick();
			break;
			
			case SDL_MOUSEBUTTONUP:
			justReleasedButtons[event.button.button] = true;
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

Vector2 Relax::getMousePosition()
{
	return mouse;
}

void Relax::runScript(const char* fileName)
{
	luaL_dofile(L, fileName);
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

void Relax::checkMouseOver()
{
	over = this;
	Element::checkMouseOver();
}

void Relax::checkClick()
{
	if (over != NULL)
		over->handleClick();
}

}


