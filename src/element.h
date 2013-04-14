#ifndef RELAX_ELEMENT_H
#define RELAX_ELEMENT_H

#include <cstdlib>
#include <list>
#include <string>
#include <map>
#include <lua5.2/lua.hpp>
#include "vector2.h"
#include "color.h"
#include "padding.h"
#include "size.h"
#include "rectangle.h"
#include "background.h"

namespace relax
{
	
class Element
{
	public:
		enum Anchor
		{
			LEFT	= 0x01,
			RIGHT   = 0x02,
			CENTERX = 0x04,
			TOP	 	= 0x08,
			BOTTOM  = 0x10,
			CENTERY = 0x20
		};
		
		typedef void (Element::*AttrSetter)(std::string attrValue);
	
	public:
		Element(std::string tag);
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
		inline Vector2 getAbsolutePosition() const { return Vector2(m_rectangle.getLeft(), m_rectangle.getTop()); }
		
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
		
		inline void setBackground(Background* background) { delete m_background; m_background = background; }
		inline void setBackgroundImage(Texture* backgroundImage) { m_background->setImage(backgroundImage); }
		inline void setBackgroundRepeat(Background::Repeat backgroundRepeat) { m_background->setRepeat(backgroundRepeat); }
		inline Background* getBackground() const { return m_background; }
		
		void setAttribute(std::string attrName, std::string attrValue);
		
		void addXML(const char* xml);
		void addXMLFile(const char* fileName);
		
		virtual void render();
		
		void checkMouseOver();
		virtual bool isMouseOver();
		
		void handleMouseDown();
		void handleMouseUp();
		void handleMouseOver();
		void handleMouseOut();
		void handleMouseMove();
		
	protected:
		// elements tree
		std::string m_tag;
		Element* m_parent;
		std::list<Element*> m_children;
		
		// graphics
		int m_anchor;
		Vector2 m_relativePosition;
		Rectangle m_rectangle;
		Color m_color;
		Size m_size;
		Padding m_padding;
		Background* m_background;
		float m_vertices[8];
		
		// lua callbacks
		int m_onMouseDown;
		int m_onMouseUp;
		int m_onMouseOver;
		int m_onMouseOut;
		int m_onMouseMove;
		
	protected:
		void saveChildTag(Element* child);
		void renderChildren();
		void draw();
		void updatePosition();
		
		static void init();
		static void quit();
		
		void checkLuaError(lua_State* L, int code);
		
		void handleEvent(int handler);
		
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
		
		void setAttrBackground(std::string attrValue);
		void setAttrBackgroundImage(std::string attrValue);
		void setAttrBackgroundRepeat(std::string attrValue);
		
		void setAttrOnEvent(std::string attrValue, int* handler);
		void setAttrOnMouseDown(std::string attrValue);
		void setAttrOnMouseUp(std::string attrValue);
		void setAttrOnMouseOver(std::string attrValue);
		void setAttrOnMouseOut(std::string attrValue);
		void setAttrOnMouseMove(std::string attrValue);
};
	
}

#endif


