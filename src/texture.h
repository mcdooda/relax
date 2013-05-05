#ifndef RELAX_TEXTURE_H
#define RELAX_TEXTURE_H

#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include "element.h"
#include "vector2.h"
#include "rectangle.h"

namespace relax
{
class Element;

class Texture
{
	protected:
		Texture();
		Texture(std::string fileName);
		virtual ~Texture();
		
	public:
		virtual bool isText() { return false; }
		virtual void update(Element* element);
		
		inline Vector2 getSize() const { return m_size; }
		inline float getWidth() const { return m_size.getX(); }
		inline float getHeight() const { return m_size.getY(); }
		
		inline void bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }
		
		static Texture* get(std::string fileName);
		
		static void init();
		static void quit();
		
	protected:
		GLuint m_id;
		Vector2 m_size;
		
	protected:
		void loadSurface(SDL_Surface* surface);
};

}

#endif


