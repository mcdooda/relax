#ifndef RELAX_BACKGROUND_H
#define RELAX_BACKGROUND_H

#include "texture.h"
#include "rectangle.h"

namespace relax
{

class Background
{
	public:
		enum Repeat
		{
			REPEAT,
			SCALE
		};
		
	public:
		Background();
		Background(Texture* image);
		
		inline void setImage(Texture* image) { m_image = image; }
		inline Texture* getImage() const { return m_image; }
		
		inline void setRepeat(Repeat repeat) { m_repeat = repeat; }
		inline Repeat getRepeat() const { return m_repeat; }
		
		inline const float* getTextureCoords() const { return m_textureCoords; }
		
		void update(const Rectangle& rectangle);
	
	private:
		Texture* m_image;
		Repeat m_repeat;
		float m_textureCoords[8];
};

}

#endif


