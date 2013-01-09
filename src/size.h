#ifndef RELAX_SIZE_H
#define RELAX_SIZE_H

namespace relax
{

class Size
{
	public:
		Size();
		
		inline void setWidth(float width) { m_width = width; m_widthAuto = false; }
		inline void setHeight(float height) { m_height = height; m_heightAuto = false; }
		inline float getWidth() const { return m_width; }
		inline float getHeight() const { return m_height; }
		
		inline void setWidthAuto() { m_widthAuto = true; }
		inline void setHeightAuto() { m_heightAuto = true; }
		inline bool getWidthAuto() const { return m_widthAuto; }
		inline bool getHeightAuto() const { return m_heightAuto; }
		
	private:
		float m_width;
		float m_height;
		bool m_widthAuto;
		bool m_heightAuto;
};

}

#endif


