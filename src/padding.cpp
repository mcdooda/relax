#include "padding.h"

namespace relax
{

Padding::Padding() :
	m_left(0),
	m_right(0),
	m_top(0),
	m_bottom(0)
{
	
}

void Padding::setAll(int padding)
{
	m_left = padding;
	m_right = padding;
	m_top = padding;
	m_bottom = padding;
}

}


