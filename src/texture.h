#ifndef RELAX_TEXTURE_H
#define RELAX_TEXTURE_H

#include <iostream>
#include <GL/gl.h>
#include "vector2.h"

namespace relax
{

class Texture
{
	private:
		Texture(std::string fileName);
		~Texture();
		
	public:
		inline Vector2 getSize() const { return m_size; }
		inline float getWidth() const { return m_size.getX(); }
		inline float getHeight() const { return m_size.getY(); }
		
		inline void bind() const { glBindTexture(GL_TEXTURE_2D, m_id); }
		
		static Texture* get(std::string fileName);
		
		static void init();
		static void free();
		
	private:
		GLuint m_id;
		Vector2 m_size;
};

}

#endif


