#ifndef RELAX_TEXTURE_H
#define RELAX_TEXTURE_H

#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include "vector2.h"

namespace relax
{

class Texture
{
	protected:
		Texture();
		Texture(std::string fileName);
		~Texture();
		
	public:
		inline Vector2 getSize() const { return m_size; }
		inline float getWidth() const { return m_size.getX(); }
		inline float getHeight() const { return m_size.getY(); }
		
		inline void bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }
		
		static Texture* get(std::string fileName);
		
		static void init();
		static void quit();
		
	private:
		GLuint m_id;
		Vector2 m_size;
		
	protected:
		void loadSurface(SDL_Surface* surface);
};

}

#endif


