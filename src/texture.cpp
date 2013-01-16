#include <map>
#include <SDL/SDL_image.h>
#include "texture.h"
#include "exception.h"

namespace relax
{

std::map<std::string, Texture*> textures;

Texture::Texture()
{
	
}

Texture::Texture(std::string fileName)
{
	SDL_Surface* surface = IMG_Load(fileName.c_str());
	
	if (surface == NULL)
		throw Exception(std::string("Error while loading image: ") + IMG_GetError());

	loadSurface(surface);
	
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
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(flags) & flags) != flags)
		throw Exception(std::string("Error while loading SDL IMG: ") + IMG_GetError());
}

void Texture::quit()
{
	for (std::map<std::string, Texture*>::iterator it = textures.begin(); it != textures.end(); it++)
		delete it->second;
	
	textures.clear();
}

void Texture::loadSurface(SDL_Surface* surface)
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);
	
	GLenum flag = GL_RGBA;
	if (surface->format->BytesPerPixel == 3)
		flag = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w, surface->h, 0, flag, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	m_size.setX(surface->w);
	m_size.setY(surface->h);
}

}


