#ifndef RELAX_FONT_H
#define RELAX_FONT_H

#include <string>
#include <SDL/SDL_ttf.h>

namespace relax
{

class Font
{
	public:
		struct Constr
		{
			std::string fileName;
			int size;
			
			bool operator<(const Constr& constr) const { return fileName < constr.fileName || (fileName == constr.fileName && size < constr.size); }
		};
		
	private:
		Font(Constr constr);
		~Font();
		
	public:
		inline TTF_Font* getFont() const { return m_font; }
		
		static Font* get(Constr constr);
		
		static void init();
		static void quit();
		
	private:
		TTF_Font* m_font;
};

}

#endif


