#include <GL/gl.h>
#include "color.h"

namespace relax
{

Color::Color() :
	m_r(255),
	m_g(255),
	m_b(255),
	m_a(255)
{
	
}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
	m_r(r),
	m_g(g),
	m_b(b),
	m_a(a)
{
	
}

void Color::use()
{
	glColor4ub(m_r, m_g, m_b, m_a);
}

}


