#ifndef RELAX_VECTOR2_H
#define RELAX_VECTOR2_H

#include <cmath>

namespace relax
{

class Vector2
{
	public:
		Vector2();
		Vector2(float x, float y);
		
		inline void setX(float x) { m_x = x; }
		inline void setY(float y) { m_y = y; }
		
		inline float getX() const { return m_x; }
		inline float getY() const { return m_y; }
		
		void setR(float r);
		inline float getR() { return sqrt(getRSq()); }
		inline float getRSq() { return m_x * m_x + m_y * m_y; }
		
		void setTheta(float theta);
		float getTheta();
		
		inline Vector2 operator+(const Vector2& v) { return Vector2(m_x + v.m_x, m_y + v.m_y); }
		inline Vector2 operator-(const Vector2& v) { return Vector2(m_x - v.m_x, m_y - v.m_y); }
		inline void operator+=(const Vector2& v) { m_x += v.m_x; m_y += v.m_y; }
		inline void operator-=(const Vector2& v) { m_x -= v.m_x; m_y -= v.m_y; }
		
	private:
		float m_x;
		float m_y;
};
	
}

#endif


