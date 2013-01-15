#include <map>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "texture.h"
#include "exception.h"

namespace relax
{

std::map<std::string, Texture*> textures;

Texture::Texture(std::string fileName)
{
	SDL_Surface* surface = IMG_Load(fileName.c_str());
	
	if (surface == NULL)
		throw Exception("Error while loading image '" + fileName + "': " + IMG_GetError());

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	m_size.setX(surface->w);
	m_size.setY(surface->h);
	
	SDL_FreeSurface(surface);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_id);
}

Texture* Texture::get(std::string fileName)
{
	std::map<std::string, Texture*>::iterator it = textures.find(fileName);
	
	if (it != textures.end())
		return it->second;
		
	else
	{
		Texture* texture = new Texture(fileName);
		textures[fileName] = texture;
		return texture;
	}
}

void Texture::init()
{
	
}

void Texture::free()
{
	for (std::map<std::string, Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
		delete it->second;
}

}
