#include "element.h"
#include "attrsetter.h"

namespace relax
{

AttrSetterAnchorX::AttrSetterAnchorX(Anchor anchorX) :
	m_anchorX(anchorX)
{
	
}

void AttrSetterAnchorX::set(Element* element)
{
	element->setAnchorX(m_anchorX);
}

AttrSetterAnchorY::AttrSetterAnchorY(Anchor anchorY) :
	m_anchorY(anchorY)
{
	
}

void AttrSetterAnchorY::set(Element* element)
{
	element->setAnchorY(m_anchorY);
}

AttrSetterSizeX::AttrSetterSizeX(float sizeX) :
	m_sizeX(sizeX)
{
	
}

void AttrSetterSizeX::set(Element* element)
{
	element->setWidth(m_sizeX);
}

void AttrSetterSizeXAuto::set(Element* element)
{
	element->setWidthAuto();
}

AttrSetterSizeY::AttrSetterSizeY(float sizeY) :
	m_sizeY(sizeY)
{
	
}

void AttrSetterSizeY::set(Element* element)
{
	element->setHeight(m_sizeY);
}

void AttrSetterSizeYAuto::set(Element* element)
{
	element->setHeightAuto();
}

AttrSetterPositionX::AttrSetterPositionX(float positionX) :
	m_positionX(positionX)
{
	
}

void AttrSetterPositionX::set(Element* element)
{
	element->setX(m_positionX);
}

AttrSetterPositionY::AttrSetterPositionY(float positionY) :
	m_positionY(positionY)
{
	
}

void AttrSetterPositionY::set(Element* element)
{
	element->setY(m_positionY);
}

AttrSetterColorRed::AttrSetterColorRed(unsigned char red) :
	m_red(red)
{
	
}

void AttrSetterColorRed::set(Element* element)
{
	element->setRed(m_red);
}

AttrSetterColorGreen::AttrSetterColorGreen(unsigned char green) :
	m_green(green)
{
	
}

void AttrSetterColorGreen::set(Element* element)
{
	element->setGreen(m_green);
}

AttrSetterColorBlue::AttrSetterColorBlue(unsigned char blue) :
	m_blue(blue)
{
	
}

void AttrSetterColorBlue::set(Element* element)
{
	element->setBlue(m_blue);
}

AttrSetterColorAlpha::AttrSetterColorAlpha(unsigned char alpha) :
	m_alpha(alpha)
{
	
}

void AttrSetterColorAlpha::set(Element* element)
{
	element->setAlpha(m_alpha);
}

AttrSetterPaddingLeft::AttrSetterPaddingLeft(float paddingLeft) :
	m_paddingLeft(paddingLeft)
{
	
}

void AttrSetterPaddingLeft::set(Element* element)
{
	element->setPaddingLeft(m_paddingLeft);
}

AttrSetterPaddingRight::AttrSetterPaddingRight(float paddingRight) :
	m_paddingRight(paddingRight)
{
	
}

void AttrSetterPaddingRight::set(Element* element)
{
	element->setPaddingRight(m_paddingRight);
}

AttrSetterPaddingTop::AttrSetterPaddingTop(float paddingTop) :
	m_paddingTop(paddingTop)
{
	
}

void AttrSetterPaddingTop::set(Element* element)
{
	element->setPaddingTop(m_paddingTop);
}

AttrSetterPaddingBottom::AttrSetterPaddingBottom(float paddingBottom) :
	m_paddingBottom(paddingBottom)
{
	
}

void AttrSetterPaddingBottom::set(Element* element)
{
	element->setPaddingBottom(m_paddingBottom);
}

AttrSetterMarginLeft::AttrSetterMarginLeft(float marginLeft) :
	m_marginLeft(marginLeft)
{
	
}

void AttrSetterMarginLeft::set(Element* element)
{
	element->setMarginLeft(m_marginLeft);
}

AttrSetterMarginRight::AttrSetterMarginRight(float marginRight) :
	m_marginRight(marginRight)
{
	
}

void AttrSetterMarginRight::set(Element* element)
{
	element->setMarginRight(m_marginRight);
}

AttrSetterMarginTop::AttrSetterMarginTop(float marginTop) :
	m_marginTop(marginTop)
{
	
}

void AttrSetterMarginTop::set(Element* element)
{
	element->setMarginTop(m_marginTop);
}

AttrSetterMarginBottom::AttrSetterMarginBottom(float marginBottom) :
	m_marginBottom(marginBottom)
{
	
}

void AttrSetterMarginBottom::set(Element* element)
{
	element->setMarginBottom(m_marginBottom);
}

AttrSetterBackgroundImage::AttrSetterBackgroundImage(Texture* backgroundImage) :
	m_backgroundImage(backgroundImage)
{
	
}

void AttrSetterBackgroundImage::set(Element* element)
{
	element->setBackgroundImage(m_backgroundImage);
}

AttrSetterBackgroundRepeat::AttrSetterBackgroundRepeat(BackgroundRepeat backgroundRepeat) :
	m_backgroundRepeat(backgroundRepeat)
{
	
}

void AttrSetterBackgroundRepeat::set(Element* element)
{
	element->setBackgroundRepeat(m_backgroundRepeat);
}

AttrSetterOnMouseDown::AttrSetterOnMouseDown(int onMouseDown) :
	m_onMouseDown(onMouseDown)
{
	
}

void AttrSetterOnMouseDown::set(Element* element)
{
	element->setOnMouseDown(m_onMouseDown);
}

AttrSetterOnMouseUp::AttrSetterOnMouseUp(int onMouseUp) :
	m_onMouseUp(onMouseUp)
{
	
}

void AttrSetterOnMouseUp::set(Element* element)
{
	element->setOnMouseUp(m_onMouseUp);
}

AttrSetterOnMouseOver::AttrSetterOnMouseOver(int onMouseOver) :
	m_onMouseOver(onMouseOver)
{
	
}

void AttrSetterOnMouseOver::set(Element* element)
{
	element->setOnMouseOver(m_onMouseOver);
}

AttrSetterOnMouseOut::AttrSetterOnMouseOut(int onMouseOut) :
	m_onMouseOut(onMouseOut)
{
	
}

void AttrSetterOnMouseOut::set(Element* element)
{
	element->setOnMouseOut(m_onMouseOut);
}

AttrSetterOnMouseMove::AttrSetterOnMouseMove(int onMouseMove) :
	m_onMouseMove(onMouseMove)
{
	
}

void AttrSetterOnMouseMove::set(Element* element)
{
	element->setOnMouseMove(m_onMouseMove);
}

}


