#ifndef RELAX_BACKGROUND_H
#define RELAX_BACKGROUND_H

#include "backgroundrepeat.h"
#include "texture.h"
#include "rectangle.h"
#include "element.h"

namespace relax
{
class Element;
class Texture;

class Background
{	
	public:
		Background();
		Background(Texture* image);
		
		inline void setImage(Texture* image) { m_image = image; }
		inline Texture* getImage() const { return m_image; }
		
		inline void setRepeat(BackgroundRepeat repeat) { m_repeat = repeat; }
		inline BackgroundRepeat getRepeat() const { return m_repeat; }
		
		inline const float* getTextureCoords() const { return m_textureCoords; }
		
		void update(Element* element);
	
	private:
		Texture* m_image;
		BackgroundRepeat m_repeat;
		float m_textureCoords[8];
		
	private:
	    void initCoords();
};

}

#endif


