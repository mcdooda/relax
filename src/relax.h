#ifndef RELAX_RELAX_H
#define RELAX_RELAX_H

#include <set>
#include <SDL/SDL.h>
#include <lua5.2/lua.hpp>
#include "element.h"
#include "exception.h"

#define K(k) SDLK_##k

namespace relax
{
typedef SDLKey Key;

class Relax : public Element
{
	public:
		static void init();
		static void quit();
		
		Relax(bool fullScreen = true);
		~Relax();
		
		inline bool isFullScreen() const { return m_fullScreen; }
		
		void pumpEvents();
		bool isPressed(Key key);
		bool isJustPressed(Key key);
		bool isJustReleased(Key key);
		
		bool operator!() const { return !m_open; }
		inline void close() { m_open = false; }
		
		virtual void render();
		
		inline void update() { updatePosition(); }
		
		void runScript(const char* fileName);
		
		Element* elementFromXML(const char* xml);
		
		void saveTag(Element* element);
		void unsaveTag(Element* element);
		
		std::set<Element*> getElementsByTag(std::string tag);
		
	private:
		bool m_open;
		bool m_fullScreen;
		std::map<std::string, std::set<Element*> > m_elementsByTag;
		lua_State* m_L;
};

}

#endif


