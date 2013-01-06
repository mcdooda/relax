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

Padding::Padding(int paddingTop, int paddingLeft, int paddingRight, int paddingBottom) :
	m_left(paddingLeft),
	m_right(paddingRight),
	m_top(paddingTop),
	m_bottom(paddingBottom)
{
	
}

}


