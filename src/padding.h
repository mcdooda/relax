#ifndef RELAX_PADDING_H
#define RELAX_PADDING_H

namespace relax
{

class Padding
{
	public:
		Padding();
		
		void setAll(int padding);
		
		inline void setLeft(int left) { m_left = left; }
		inline void setRight(int right) { m_right = right; }
		inline void setTop(int top) { m_top = top; }
		inline void setBottom(int bottom) { m_bottom = bottom; }
		inline int getLeft() const { return m_left; }
		inline int getRight() const { return m_right; }
		inline int getTop() const { return m_top; }
		inline int getBottom() const { return m_bottom; }
		
	private:
		int m_left;
		int m_right;
		int m_top;
		int m_bottom;
};

}

#endif


