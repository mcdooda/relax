#include "text.h"
#include "exception.h"

namespace relax
{

Text::Text(Font* font, Color color, std::string text) :
	m_font(font),
	m_color(color),
	m_text(text)
{
	SDL_Color sdlColor = { color.getBlue(), color.getGreen(), color.getRed() };
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font->getFont(), text.c_str(), sdlColor);
    
    if (surface == NULL)
		throw Exception(std::string("Error while rendering text: ") + TTF_GetError());
		
	loadSurface(surface);
	
	SDL_FreeSurface(surface);
}

Text::~Text()
{
	
}

}


