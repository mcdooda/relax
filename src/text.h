#ifndef RELAX_TEXT_H
#define RELAX_TEXT_H

#include <vector>
#include "texture.h"
#include "font.h"
#include "color.h"

namespace relax
{

class Text : public Texture
{
	public:
		Text(Font* font, Color color, const Rectangle& rectangle);
		virtual ~Text();
		
		void appendString(const std::string& string);
		
		virtual bool isText() { return true; }
		virtual void update(Element* element);
		
		inline const std::string& getString() const { return m_text; }
		
		static std::string trim(const std::string& str);
		static std::vector<std::string> splitLines(const std::string& str);
		static std::vector<std::string> splitTokens(const std::string& str);
		
	private:
		Font* m_font;
		Color m_color;
		std::string m_text;
};

}

#endif


