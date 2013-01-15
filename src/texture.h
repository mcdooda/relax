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
		
		static Texture* get(std::string fileName);
		
		static void init();
		static void free();
		
	private:
		GLuint m_id;
		Vector2 m_size;
};

}

#endif


