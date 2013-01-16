#ifndef RELAX_TEXT_H
#define RELAX_TEXT_H

#include "texture.h"
#include "font.h"
#include "color.h"

namespace relax
{

class Text : Texture
{
	public:
		Text(Font* font, Color color, std::string text);
		~Text();
		
	private:
		Font* m_font;
		Color m_color;
		std::string m_text;
};

}

#endif


