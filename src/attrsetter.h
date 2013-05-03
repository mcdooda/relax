#ifndef RELAX_ATTRSETTER_H
#define RELAX_ATTRSETTER_H

#include <list>
#include "element.h"
#include "background.h"
#include "anchor.h"

namespace relax
{
class Element;

class AttrSetter
{
	public:
		virtual ~AttrSetter() {}
		
		virtual void set(Element* element) = 0;
};

template <int N>
class AttrSetterList : public AttrSetter
{
	public:
		virtual ~AttrSetterList()
		{
			for (int i = 0; i < N; i++)
				delete m_setters[i];
		}
	
		virtual void set(Element* element)
		{
			for (int i = 0; i < N; i++)
				m_setters[i]->set(element);
		}
		
		void add(int index, AttrSetter* setter)
		{
			m_setters[index] = setter;
		}
		
	private:
		AttrSetter* m_setters[N];
};

class AttrSetterAnchorX : public AttrSetter
{
	public:
		AttrSetterAnchorX(Anchor anchorX);
		
		virtual void set(Element* element);
		
	private:
		Anchor m_anchorX;
};

class AttrSetterAnchorY : public AttrSetter
{
	public:
		AttrSetterAnchorY(Anchor anchorY);
		
		virtual void set(Element* element);
		
	private:
		Anchor m_anchorY;
};

class AttrSetterSizeX : public AttrSetter
{
	public:
		AttrSetterSizeX(float sizeX);
		
		virtual void set(Element* element);
		
	private:
		float m_sizeX;
};

class AttrSetterSizeXAuto : public AttrSetter
{
	public:
		
		virtual void set(Element* element);
};

class AttrSetterSizeY : public AttrSetter
{
	public:
		AttrSetterSizeY(float sizeY);
		
		virtual void set(Element* element);
		
	private:
		float m_sizeY;
};

class AttrSetterSizeYAuto : public AttrSetter
{
	public:
		
		virtual void set(Element* element);
};

class AttrSetterPositionX : public AttrSetter
{
	public:
		AttrSetterPositionX(float positionX);
		
		virtual void set(Element* element);
		
	private:
		float m_positionX;
};

class AttrSetterPositionY : public AttrSetter
{
	public:
		AttrSetterPositionY(float positionY);
		
		virtual void set(Element* element);
		
	private:
		float m_positionY;
};

class AttrSetterColorRed : public AttrSetter
{
	public:
		AttrSetterColorRed(unsigned char red);
		
		virtual void set(Element* element);
		
	private:
		unsigned char m_red;
};

class AttrSetterColorGreen : public AttrSetter
{
	public:
		AttrSetterColorGreen(unsigned char green);
		
		virtual void set(Element* element);
		
	private:
		unsigned char m_green;
};

class AttrSetterColorBlue : public AttrSetter
{
	public:
		AttrSetterColorBlue(unsigned char blue);
		
		virtual void set(Element* element);
		
	private:
		unsigned char m_blue;
};

class AttrSetterColorAlpha : public AttrSetter
{
	public:
		AttrSetterColorAlpha(unsigned char alpha);
		
		virtual void set(Element* element);
		
	private:
		unsigned char m_alpha;
};

class AttrSetterPaddingLeft : public AttrSetter
{
	public:
		AttrSetterPaddingLeft(float paddingLeft);
		
		virtual void set(Element* element);
		
	private:
		float m_paddingLeft;
};

class AttrSetterPaddingRight : public AttrSetter
{
	public:
		AttrSetterPaddingRight(float paddingRight);
		
		virtual void set(Element* element);
		
	private:
		float m_paddingRight;
};

class AttrSetterPaddingTop : public AttrSetter
{
	public:
		AttrSetterPaddingTop(float paddingTop);
		
		virtual void set(Element* element);
		
	private:
		float m_paddingTop;
};

class AttrSetterPaddingBottom : public AttrSetter
{
	public:
		AttrSetterPaddingBottom(float paddingBottom);
		
		virtual void set(Element* element);
		
	private:
		float m_paddingBottom;
};

class AttrSetterBackgroundImage : public AttrSetter
{
	public:
		AttrSetterBackgroundImage(Texture* backgroundImage);
		
		virtual void set(Element* element);
		
	private:
		Texture* m_backgroundImage;
};

class AttrSetterBackgroundRepeat : public AttrSetter
{
	public:
		AttrSetterBackgroundRepeat(Background::Repeat backgroundRepeat);
		
		virtual void set(Element* element);
		
	private:
		Background::Repeat m_backgroundRepeat;
};

class AttrSetterOnMouseDown : public AttrSetter
{
	public:
		AttrSetterOnMouseDown(int onMouseDown);
		
		virtual void set(Element* element);
		
	private:
		int m_onMouseDown;
};

class AttrSetterOnMouseUp : public AttrSetter
{
	public:
		AttrSetterOnMouseUp(int onMouseDown);
		
		virtual void set(Element* element);
		
	private:
		int m_onMouseUp;
};

class AttrSetterOnMouseOver : public AttrSetter
{
	public:
		AttrSetterOnMouseOver(int onMouseOver);
		
		virtual void set(Element* element);
		
	private:
		int m_onMouseOver;
};

class AttrSetterOnMouseOut : public AttrSetter
{
	public:
		AttrSetterOnMouseOut(int onMouseOut);
		
		virtual void set(Element* element);
		
	private:
		int m_onMouseOut;
};

class AttrSetterOnMouseMove : public AttrSetter
{
	public:
		AttrSetterOnMouseMove(int onMouseMove);
		
		virtual void set(Element* element);
		
	private:
		int m_onMouseMove;
};

}

#endif


