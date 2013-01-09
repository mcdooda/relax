#ifndef RELAX_ROTATION_H
#define RELAX_ROTATION_H

#include "vector2.h"

namespace relax
{

class Rotation
{
	public:
		Rotation();
		Rotation(Vector2 center, float angle);
		
		inline void setCenter(Vector2 center) { m_center = center; }
		inline void setCenterX(float centerX) { m_center.setX(centerX); }
		inline void setCenterY(float centerY) { m_center.setY(centerY); }
		inline Vector2 getCenter() const { return m_center; }
		
		inline void setAngle(float angle) { m_angle = angle; }
		inline float getAngle() const { return m_angle; }
		
		Vector2 getRotatedPoint(Vector2 point);
		
	private:
		Vector2 m_center;
		float m_angle;
};

}

#endif


