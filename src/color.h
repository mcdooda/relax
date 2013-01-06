#ifndef RELAX_COLOR_H
#define RELAX_COLOR_H

namespace relax
{

class Color
{
	public:
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255);
		
		inline void setRed(unsigned char r) { m_r = r; }
		inline void setGreen(unsigned char g) { m_g = g; }
		inline void setBlue(unsigned char b) { m_b = b; }
		inline void setAlpha(unsigned char a) { m_a = a; }
		inline unsigned char getRed() const { return m_r; }
		inline unsigned char getGreen() const { return m_g; }
		inline unsigned char getBlue() const { return m_b; }
		inline unsigned char getAlpha() const { return m_a; }
		
		void use();
		
	private:
		unsigned char m_r;
		unsigned char m_g;
		unsigned char m_b;
		unsigned char m_a;
};

}

#endif


