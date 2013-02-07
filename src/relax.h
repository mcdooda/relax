#ifndef RELAX_RELAX_H
#define RELAX_RELAX_H

#include <set>
#include <SDL/SDL.h>
#include <lua5.2/lua.hpp>
#include "element.h"
#include "exception.h"
#include "xml.h"

#define K(k) SDLK_##k

namespace relax
{
typedef SDLKey Key;

class Relax : public Element
{
	public:
		static void init();
		static void quit();
		
		static Vector2 getDesktopResolution();
		
		Relax(Vector2 size = getDesktopResolution(), bool fullScreen = true, bool resizable = false);
		~Relax();
		
		void pumpEvents();
		bool isPressed(Key key);
		bool isJustPressed(Key key);
		bool isJustReleased(Key key);
		
		bool operator!() const { return !m_open; }
		inline void close() { m_open = false; }
		
		virtual void render();
		
		inline void update() { updatePosition(); }
		
		void runScript(const char* fileName);
		
		void saveTag(Element* element);
		void unsaveTag(Element* element);
		
		std::set<Element*> getElementsByTag(std::string tag);
		
	private:
		Uint32 m_videoFlags;
		bool m_open;
		std::map<std::string, std::set<Element*> > m_elementsByTag;
		lua_State* m_L;
		
	private:
		void updateSize(Vector2 newSize);
};

}

#endif


