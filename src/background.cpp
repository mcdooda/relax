#include "background.h"

namespace relax
{

Background::Background() :
	m_repeat(SCALE)
{
	m_textureCoords[0] = 0; m_textureCoords[1] = 0;
	m_textureCoords[2] = 1; m_textureCoords[3] = 0;
	m_textureCoords[4] = 1; m_textureCoords[5] = 1;
	m_textureCoords[6] = 0; m_textureCoords[7] = 1;
}

Background::Background(Texture* image) :
	Background()
{
	m_image = image;
}

void Background::update(const Rectangle& rectangle)
{
	switch (m_repeat)
	{
		case REPEAT:
		{
			float rX = rectangle.getWidth() / m_image->getWidth();
			float rY = rectangle.getHeight() / m_image->getHeight();
			
			m_textureCoords[0] = 0;  m_textureCoords[1] = 0;
			m_textureCoords[2] = rX; m_textureCoords[3] = 0;
			m_textureCoords[4] = rX; m_textureCoords[5] = rY;
			m_textureCoords[6] = 0;  m_textureCoords[7] = rY;
		}
		break;
		
		case SCALE:
		{
			m_textureCoords[0] = 0;  m_textureCoords[1] = 0;
			m_textureCoords[2] = 1; m_textureCoords[3] = 0;
			m_textureCoords[4] = 1; m_textureCoords[5] = 1;
			m_textureCoords[6] = 0;  m_textureCoords[7] = 1;
		}
		break;
	}
}

}


