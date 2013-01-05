#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "relax.h"

namespace relax
{

void Relax::init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw Exception(SDL_GetError());
	}
	Element::init();
}

void Relax::quit()
{
	SDL_Quit();
}

Relax::Relax(bool fullScreen) :
	Element(),
	m_open(true),
	m_fullScreen(fullScreen)
{
	const SDL_VideoInfo* videoInfo = SDL_GetVideoInfo();
	Uint32 flags = SDL_OPENGL;
	
	if (fullScreen)
		flags |= SDL_FULLSCREEN;
	
	m_realSize.setX(videoInfo->current_w);
	m_realSize.setY(videoInfo->current_h);
	m_size.setWidth(m_realSize.getX());
	m_size.setHeight(m_realSize.getY());
	SDL_SetVideoMode(m_realSize.getX(), m_realSize.getY(), videoInfo->vfmt->BitsPerPixel, flags);
	glViewport(0, 0, m_realSize.getX(), m_realSize.getY());
	gluOrtho2D(0, m_realSize.getX(), m_realSize.getY(), 0);
}

void Relax::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	renderChildren();
	glFlush();
	SDL_GL_SwapBuffers();
}

Relax::~Relax()
{
	
}

char* Relax::pumpEvents()
{
	SDL_PumpEvents();
	return (char*) SDL_GetKeyState(NULL);
}

}


