#include <cmath>
#include "vector2.h"
#include "pi.h"

namespace relax
{

Vector2::Vector2() :
	m_x(0),
	m_y(0)
{
	
}

Vector2::Vector2(float x, float y) :
	m_x(x),
	m_y(y)
{

}

void Vector2::setR(float r)
{
	float currentR = getR();
	float ratio = r / currentR;
	m_x *= ratio;
	m_y *= ratio;
}

float Vector2::getR()
{
	return sqrt(m_x * m_x + m_y * m_y);
}

float Vector2::getRSq()
{
	return m_x * m_x + m_y * m_y;
}

void Vector2::setTheta(float theta)
{
	float r = getR();
	m_x = cos(theta) * r;
	m_y = sin(theta) * r;
}

float Vector2::getTheta()
{
	if (m_x < 0)
	{
		return atan(m_y / m_x) + PI;
	}
	else if (m_x > 0)
	{
		return atan(m_y / m_x);
	}
	else
	{
		if (m_y < 0)
			return -PI / 2;
			
		else if (m_y > 0)
			return PI / 2;
			
		else
			return 0;
	}
}

}


