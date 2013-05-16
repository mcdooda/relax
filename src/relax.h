#ifndef RELAX_RELAX_H
#define RELAX_RELAX_H

#include <set>
#include <SDL/SDL.h>
#include "element.h"
#include "exception.h"
#include "xml.h"

#define K(k) SDLK_##k

#define RELAX_NUM_BUTTONS 8

namespace relax
{
typedef SDLKey Key;

class Relax : public Element
{
	public:
		static void init(lua_State* L1);
		static void quit();
		static void setLuaState(lua_State* L1);
		static lua_State* getLuaState();
		static Vector2 getDesktopResolution();
		
		/* private api */
		static void saveTag(Element* element);
		static void unsaveTag(Element* element);
		static std::set<Element*> getElementsByTagName(std::string tag);
		inline static void setOverElement(Element* element) { over = element; }
		
		/* public */
		
		Relax(Vector2 size = getDesktopResolution(), bool fullScreen = true, bool resizable = false);
		~Relax();
		
		void pumpEvents();
		bool isPressed(Key key);
		bool isJustPressed(Key key);
		bool isJustReleased(Key key);
		bool isMousePressed(int button);
		bool isMouseJustPressed(int button);
		bool isMouseJustReleased(int button);
		inline static Vector2 getMouse() { return mouse; }
		
		bool operator!() const { return !m_open; }
		inline void close() { m_open = false; }
		
		virtual void render();
		
		virtual bool isMouseOver();
		
		inline void update() { updatePosition(NULL); }
		
		void runScript(const char* fileName);
		
	private:
		/* attributes */
		Uint32 m_videoFlags;
		bool m_open;
		
	private:
		/* methods */
		void updateSize(Vector2 newSize);
		
		void checkMouseDown();
		void checkMouseUp();
		void checkMouseOver();
		void checkMouseMove();
		
	private:
		/* static */
		static lua_State* L;
		static std::map<std::string, std::set<Element*> > elementsByTagName;
		static bool justPressedKeys[SDLK_LAST];
		static bool justReleasedKeys[SDLK_LAST];
		static bool justPressedButtons[RELAX_NUM_BUTTONS];
		static bool justReleasedButtons[RELAX_NUM_BUTTONS];
		static Vector2 mouse;
		static Element* over;
		static Element* previousOver;
};

}

#endif


