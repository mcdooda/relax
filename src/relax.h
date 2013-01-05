#ifndef RELAX_RELAX_H
#define RELAX_RELAX_H

#include "element.h"
#include "exception.h"

namespace relax
{

class Relax : public Element
{
	public:
		static void init();
		static void quit();
		
		Relax(bool fullScreen = true);
		~Relax();
		
		inline bool isFullScreen() const { return m_fullScreen; }
		
		char* pumpEvents();
		
		bool operator!() const { return !m_open; }
		inline void close() { m_open = false; }
		
		virtual void render();
		
		inline void update() { updatePosition(); }
		
	private:
		bool m_open;
		bool m_fullScreen;
};

}

#endif


