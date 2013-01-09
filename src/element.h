#ifndef RELAX_ELEMENT_H
#define RELAX_ELEMENT_H

#include <cstdlib>
#include <list>
#include <string>
#include <map>
#include "vector2.h"
#include "color.h"
#include "padding.h"
#include "size.h"
#include "rectangle.h"
#include "rotation.h"

namespace relax
{
	
class Element
{
	public:
		enum Anchor
		{
			LEFT    = 0x01,
			RIGHT   = 0x02,
			CENTERX = 0x04,
			TOP     = 0x08,
			BOTTOM  = 0x10,
			CENTERY = 0x20
		};
		
		typedef void (Element::*AttrSetter)(std::string attrValue);
	
	protected:
		Element(std::string tag);
		
	public:
		Element(std::string tag, Element* window);
		virtual ~Element();
		
		inline const std::string& getTag() const { return m_tag; }
		
		void addChild(Element* child);
		inline Element* getParent() { return m_parent; }
		inline const std::list<Element*>& getChildren() { return m_children; }
		
		inline void setAnchor(int anchor) { m_anchor = anchor; }
		inline void setAnchorX(Anchor anchor) { m_anchor = (m_anchor & ~(LEFT | RIGHT | CENTERX)) | anchor; }
		inline void setAnchorY(Anchor anchor) { m_anchor = (m_anchor & ~(TOP | BOTTOM | CENTERY)) | anchor; }
		
		inline void setSize(Size size) { m_size = size; }
		inline void setWidthAuto() { m_size.setWidthAuto(); }
		inline void setHeightAuto() { m_size.setHeightAuto(); }
		inline void setWidth(float width) { m_size.setWidth(width); }
		inline void setHeight(float height) { m_size.setHeight(height); }
		inline Size getSize() const { return m_size; }
		inline Vector2 getComputedSize() const { return m_rectangle.getSize(); }
		inline float getComputedWidth() const { return m_rectangle.getWidth(); }
		inline float getComputedHeight() const { return m_rectangle.getHeight(); }
		
		inline void setPosition(Vector2 position) { m_relativePosition = position; }
		inline void setX(float x) { m_relativePosition.setX(x); }
		inline void setY(float y) { m_relativePosition.setY(y); }
		inline Vector2 getPosition() const { return m_relativePosition; }
		
		/*
		inline void setRotation(Rotation rotation) { m_rotation = rotation; }
		inline void setRotationCenter(Vector2 rotationCenter) { m_rotation.setCenter(rotationCenter); }
		inline void setRotationCenterX(float centerX) { m_rotation.setCenterX(centerX); }
		inline void setRotationCenterY(float centerY) { m_rotation.setCenterY(centerY); }
		inline void setRotationAngle(float angle) { m_rotation.setAngle(angle); }
		inline Rotation getRotation() const { return m_rotation; }
		*/
		
		inline void setColor(Color color) { m_color = color; }
		inline void setRed(unsigned char r) { m_color.setRed(r); }
		inline void setGreen(unsigned char g) { m_color.setGreen(g); }
		inline void setBlue(unsigned char b) { m_color.setBlue(b); }
		inline void setAlpha(unsigned char a) { m_color.setAlpha(a); }
		inline Color getColor() const { return m_color; }
		
		inline void setPadding(Padding padding) { m_padding = padding; }
		inline void setPaddingLeft(float paddingLeft) { m_padding.setLeft(paddingLeft); }
		inline void setPaddingRight(float paddingRight) { m_padding.setRight(paddingRight); }
		inline void setPaddingTop(float paddingTop) { m_padding.setTop(paddingTop); }
		inline void setPaddingBottom(float paddingBottom) { m_padding.setBottom(paddingBottom); }
		inline Padding getPadding() const { return m_padding; }
		
		void setAttribute(std::string attrName, std::string attrValue);
		
		virtual void render();
		
	protected:
		Element* m_window;
		std::string m_tag;
		Element* m_parent;
		std::list<Element*> m_children;
		int m_anchor;
		Vector2 m_relativePosition;
		Rectangle m_rectangle;
		//Rotation m_rotation;
		Color m_color;
		Size m_size;
		Padding m_padding;
		
		float m_vertices[8];
		
	protected:
		void renderChildren();
		void draw();
		void updatePosition();
		
		static void init();
		
		// attribute modifiers
		void setAttrAnchor(std::string attrValue);
		void setAttrAnchorX(std::string attrValue);
		void setAttrAnchorY(std::string attrValue);
		
		void setAttrSize(std::string attrValue);
		void setAttrSizeX(std::string attrValue);
		void setAttrSizeY(std::string attrValue);
		
		void setAttrPosition(std::string attrValue);
		void setAttrPositionX(std::string attrValue);
		void setAttrPositionY(std::string attrValue);
		
		/*
		void setAttrRotation(std::string attrValue);
		void setAttrRotationCenter(std::string attrValue);
		void setAttrRotationCenterX(std::string attrValue);
		void setAttrRotationCenterY(std::string attrValue);
		void setAttrRotationAngle(std::string attrValue);
		*/
		
		void setAttrColor(std::string attrValue);
		void setAttrColorRed(std::string attrValue);
		void setAttrColorGreen(std::string attrValue);
		void setAttrColorBlue(std::string attrValue);
		void setAttrColorAlpha(std::string attrValue);
		
		void setAttrPadding(std::string attrValue);
		void setAttrPaddingLeft(std::string attrValue);
		void setAttrPaddingRight(std::string attrValue);
		void setAttrPaddingTop(std::string attrValue);
		void setAttrPaddingBottom(std::string attrValue);
};
	
}

#endif


