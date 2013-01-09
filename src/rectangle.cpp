#include "rectangle.h"

namespace relax
{

Rectangle::Rectangle() :
	m_left(0),
	m_right(0),
	m_top(0),
	m_bottom(0)
{
	
}

Rectangle::Rectangle(float top, float left, float right, float bottom) :
	m_left(left),
	m_right(right),
	m_top(top),
	m_bottom(bottom)
{
	
}

void Rectangle::copyToVertices(float* vertices)
{
	vertices[0] = m_left;  vertices[1] = m_top;
	vertices[2] = m_right; vertices[3] = m_top;
	vertices[4] = m_right; vertices[5] = m_bottom;
	vertices[6] = m_left;  vertices[7] = m_bottom;
}

bool Rectangle::contains(Vector2 point)
{
	return m_left <= point.getX() && point.getX() <= m_right && m_top <= point.getY() && point.getY() <= m_bottom;
}

}


