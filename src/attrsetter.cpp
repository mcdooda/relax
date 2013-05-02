#include "attrsetter.h"

namespace relax
{

void AttrSetterList::set(Element* element)
{
	for (std::list<AttrSetter*>::iterator it = m_setters.begin(); it != m_setters.end(); it++)
	{
		(*it)->set(element);
	}
}

void AttrSetterList::add(AttrSetter* setter)
{
	m_setters.push_back(setter);
}

AttrSetterAnchorX::AttrSetterAnchorX(Element::Anchor anchorX) :
	m_anchorX(anchorX)
{
	
}

void AttrSetterAnchorX::set(Element* element)
{
	
}

AttrSetterAnchorY::AttrSetterAnchorY(Element::Anchor anchorY) :
	m_anchorY(anchorY)
{
	
}

void AttrSetterAnchorY::set(Element* element)
{
	
}

AttrSetterSizeX::AttrSetterSizeX() :
	m_sizeX(0),
	m_auto(true)
{
	
}

AttrSetterSizeX::AttrSetterSizeX(float sizeX) :
	m_sizeX(sizeX),
	m_auto(false)
{
	
}

void AttrSetterSizeX::set(Element* element)
{
	
}

AttrSetterSizeY::AttrSetterSizeY() :
	m_sizeY(0),
	m_auto(true)
{
	
}

AttrSetterSizeY::AttrSetterSizeY(float sizeY) :
	m_sizeY(sizeY),
	m_auto(false)
{
	
}

void AttrSetterSizeY::set(Element* element)
{
	
}

AttrSetterPositionX::AttrSetterPositionX(float positionX) :
	m_positionX(positionX)
{
	
}

void AttrSetterPositionX::set(Element* element)
{
	
}

AttrSetterPositionY::AttrSetterPositionY(float positionY) :
	m_positionY(positionY)
{
	
}

void AttrSetterPositionY::set(Element* element)
{
	
}

AttrSetterColorRed::AttrSetterColorRed(unsigned char red) :
	m_red(red)
{
	
}

void AttrSetterColorRed::set(Element* element)
{
	
}

AttrSetterColorGreen::AttrSetterColorGreen(unsigned char green) :
	m_green(green)
{
	
}

void AttrSetterColorGreen::set(Element* element)
{
	
}

AttrSetterColorBlue::AttrSetterColorBlue(unsigned char blue) :
	m_blue(blue)
{
	
}

void AttrSetterColorBlue::set(Element* element)
{
	
}

AttrSetterColorAlpha::AttrSetterColorAlpha(unsigned char alpha) :
	m_alpha(alpha)
{
	
}

void AttrSetterColorAlpha::set(Element* element)
{
	
}

AttrSetterPaddingLeft::AttrSetterPaddingLeft(float paddingLeft) :
	m_paddingLeft(paddingLeft)
{
	
}

void AttrSetterPaddingLeft::set(Element* element)
{
	
}

AttrSetterPaddingRight::AttrSetterPaddingRight(float paddingRight) :
	m_paddingRight(paddingRight)
{
	
}

void AttrSetterPaddingRight::set(Element* element)
{
	
}

AttrSetterPaddingTop::AttrSetterPaddingTop(float paddingTop) :
	m_paddingTop(paddingTop)
{
	
}

void AttrSetterPaddingTop::set(Element* element)
{
	
}

AttrSetterPaddingBottom::AttrSetterPaddingBottom(float paddingBottom) :
	m_paddingBottom(paddingBottom)
{
	
}

void AttrSetterPaddingBottom::set(Element* element)
{
	
}

AttrSetterBackgroundImage::AttrSetterBackgroundImage(Texture* backgroundImage) :
	m_backgroundImage(backgroundImage)
{
	
}

void AttrSetterBackgroundImage::set(Element* element)
{
	
}

AttrSetterBackgroundRepeat::AttrSetterBackgroundRepeat(Background::Repeat backgroundRepeat) :
	m_backgroundRepeat(backgroundRepeat)
{
	
}

void AttrSetterBackgroundRepeat::set(Element* element)
{
	
}

AttrSetterOnMouseDown::AttrSetterOnMouseDown(int onMouseDown) :
	m_onMouseDown(onMouseDown)
{
	
}

void AttrSetterOnMouseDown::set(Element* element)
{
	
}

AttrSetterOnMouseUp::AttrSetterOnMouseUp(int onMouseUp) :
	m_onMouseUp(onMouseUp)
{
	
}

void AttrSetterOnMouseUp::set(Element* element)
{
	
}

AttrSetterOnMouseOver::AttrSetterOnMouseOver(int onMouseOver) :
	m_onMouseOver(onMouseOver)
{
	
}

void AttrSetterOnMouseOver::set(Element* element)
{
	
}

AttrSetterOnMouseOut::AttrSetterOnMouseOut(int onMouseOut) :
	m_onMouseOut(onMouseOut)
{
	
}

void AttrSetterOnMouseOut::set(Element* element)
{
	
}

AttrSetterOnMouseMove::AttrSetterOnMouseMove(int onMouseMove) :
	m_onMouseMove(onMouseMove)
{
	
}

void AttrSetterOnMouseMove::set(Element* element)
{
	
}

}


