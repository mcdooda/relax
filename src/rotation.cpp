#include <cmath>
#include "rotation.h"

namespace relax
{

Rotation::Rotation() :
	m_center(Vector2()),
	m_angle(0)
{
	
}

Rotation::Rotation(Vector2 center, float angle) :
	m_center(center),
	m_angle(angle)
{
	
}

Vector2 Rotation::getRotatedPoint(Vector2 point)
{
	if (m_angle == 0)
		return point;
		
	Vector2 v = m_center - point;
	v.setTheta(v.getTheta() + m_angle);
	return m_center + v;
}

}


