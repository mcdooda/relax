#include <algorithm>
#include <iterator>
#include <sstream>
#include "text.h"
#include "exception.h"

namespace relax
{

Text::Text(Font* font, Color color, const Rectangle& rectangle) :
	m_font(font),
	m_color(color),
	m_text("")
{
	
}

Text::~Text()
{
	
}

void Text::appendString(const std::string& string)
{
	m_text += string;
}

void Text::update(Element* element)
{
	const Rectangle& rectangle = element->getRectangle();
	if (rectangle.getWidth() > 0 && rectangle.getHeight() > 0)
	{
		SDL_Color sdlColor = { m_color.getBlue(), m_color.getGreen(), m_color.getRed() };
		
		Uint32 rmask, gmask, bmask, amask;

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
		#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
		#endif
		
		SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, (int) rectangle.getWidth(), (int) rectangle.getHeight(), 32, rmask, gmask, bmask, amask);
		SDL_FillRect(surface, NULL, SDL_MapRGBA(surface->format, 255, 255, 255, 128));
		
		TTF_Font* font = m_font->getFont();
		
		std::vector<std::string> tokens = splitTokens(m_text);
		
		int lineHeight = TTF_FontLineSkip(font);
		int spaceWidth;
		TTF_SizeUTF8(font, " ", &spaceWidth, NULL);
		
		SDL_Rect pos;
		pos.x = 0;
		pos.y = 0;
		for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
		{
			const char* token = it->c_str();
			if (token[0] != '\0')
			{
				int width, height;
				TTF_SizeUTF8(font, token, &width, &height);
			
				if (pos.x > 0 && pos.x + width > rectangle.getWidth())
				{
					pos.x = 0;
					pos.y += lineHeight;
					if (pos.y > rectangle.getHeight())
						break;
				}
			
				SDL_Surface* tokenSurface = TTF_RenderUTF8_Blended(font, token, sdlColor);
				SDL_BlitSurface(tokenSurface, NULL, surface, &pos);
				SDL_FreeSurface(tokenSurface);
			
				pos.x += width + spaceWidth;
			}
		}
		
		loadSurface(surface);
	
		SDL_FreeSurface(surface);
	}
}

std::string Text::trim(const std::string& str)
{
	std::string s = str;
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::vector<std::string> Text::splitLines(const std::string& str)
{
	std::vector<std::string> lines;
    std::stringstream ss(str);
    std::string line;
    while (std::getline(ss, line)) {
        lines.push_back(trim(line));
    }
    return lines;
}

std::vector<std::string> Text::splitTokens(const std::string& str)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::copy(
		std::istream_iterator<std::string>(ss),
		std::istream_iterator<std::string>(),
		std::back_inserter<std::vector<std::string> >(tokens)
	);
	return tokens;
}

}


