#ifndef RELAX_COLOR_H
#define RELAX_COLOR_H

namespace relax
{

class Color
{
	public:
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		
		void use();
		
	private:
		unsigned char m_r;
		unsigned char m_g;
		unsigned char m_b;
		unsigned char m_a;
};

}

#endif


