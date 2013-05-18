#include <map>
#include "font.h"
#include "exception.h"

namespace relax
{

Font* Font::defaultFont;

std::map<Font::Constr, Font*> fonts;

Font::Font(Constr constr)
{
	m_font = TTF_OpenFont(constr.fileName.c_str(), constr.size);
	if (m_font == NULL)
		throw Exception(std::string("Error while loading font: ") + TTF_GetError());
}

Font::~Font()
{
	TTF_CloseFont(m_font);
}

Font* Font::get(Constr constr)
{
	std::map<Constr, Font*>::iterator it = fonts.find(constr);
	
	if (it != fonts.end())
		return it->second;
		
	else
	{
		Font* font = new Font(constr);
		fonts[constr] = font;
		return font;
	}
}

void Font::init()
{
	if (TTF_Init() == -1)
		throw Exception(std::string("Error while loading SDL TTF: ") + TTF_GetError());
	
	Constr constr;
	constr.fileName = "/usr/share/fonts/truetype/freefont/FreeSans.ttf";
	constr.size = 20;
	
	defaultFont = get(constr);
}

void Font::quit()
{
	for (std::map<Constr, Font*>::iterator it = fonts.begin(); it != fonts.end(); it++)
		delete it->second;
		
	fonts.clear();
}

}


