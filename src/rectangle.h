#ifndef RELAX_RECTANGLE_H
#define RELAX_RECTANGLE_H

#include "vector2.h"

namespace relax
{

class Rectangle
{
	public:
		Rectangle();
		Rectangle(float top, float left, float right, float bottom);
		
		inline void setLeft(float left) { m_left = left; }
		inline void setRight(float right) { m_right = right; }
		inline void setTop(float top) { m_top = top; }
		inline void setBottom(float bottom) { m_bottom = bottom; }
		inline float getLeft() const { return m_left; }
		inline float getRight() const { return m_right; }
		inline float getTop() const { return m_top; }
		inline float getBottom() const { return m_bottom; }
		
		inline float getWidth() const { return m_right - m_left; }
		inline float getHeight() const { return m_bottom - m_top; }
		inline Vector2 getSize() const { return Vector2(getWidth(), getHeight()); }
		
		inline float getCenterX() const { return (m_left + m_right) / 2; }
		inline float getCenterY() const { return (m_top + m_bottom) / 2; }
		inline Vector2 getCenter() const { return Vector2(getCenterX(), getCenterY()); }
		
		void copyToVertices(float* vertices);
		
		bool contains(Vector2 point);
		
	private:
		float m_left;
		float m_right;
		float m_top;
		float m_bottom;
};

}

#endif


