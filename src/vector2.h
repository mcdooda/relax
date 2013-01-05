#ifndef RELAX_VECTOR2_H
#define RELAX_VECTOR2_H

namespace relax
{

class Vector2
{
	public:
		Vector2(int x, int y);
		Vector2();
		
		inline void setX(int x) { m_x = x; }
		inline void setY(int y) { m_y = y; }
		
		inline int getX() const { return m_x; }
		inline int getY() const { return m_y; }
		
		inline Vector2 operator+(const Vector2& v) { return Vector2(m_x + v.m_x, m_y + v.m_y); }
		
	private:
		int m_x;
		int m_y;
};
	
}

#endif


