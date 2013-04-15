#include "element.h"
#include "../relax.h"

namespace relax
{
namespace api
{
namespace element
{

static const struct luaL_Reg lib_m[] = {
    {"__tostring",          toString},
    {"getTag",              getTag},
    {"setAttribute",        setAttribute},
    {"getSize",             getSize},
    {"getAbsolutePosition", getAbsolutePosition},
    {"getPosition",         getPosition},
    {NULL, NULL}
};

static const struct luaL_Reg lib_f[] = {
	{"getByTagName", getByTagName},
	{"style",        style},
	{NULL, NULL}
};

void open(lua_State* L)
{
    luaL_newmetatable(L, "Relax.Element");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, lib_m, 0);
    lua_pop(L, 1);
    
    luaL_newlib(L, lib_f);
    lua_setglobal(L, "element");
}

void newRef(lua_State* L, Element* element)
{
    Element** e = (Element**) lua_newuserdata(L, sizeof(Element*));
    luaL_getmetatable(L, "Relax.Element");
    lua_setmetatable(L, -2);
    *e = element;
}

#define checkElement(L) (*(Element**) luaL_checkudata(L, 1, "Relax.Element"))

int toString(lua_State* L)
{
    Element* element = checkElement(L);
    lua_pushfstring(L, "<%s /> (%p)", element->getTag().c_str(), element);
    return 1;
}

int getTag(lua_State* L)
{
    Element* element = checkElement(L);
    lua_pushstring(L, element->getTag().c_str());
    return 1;
}

int setAttribute(lua_State* L)
{
	Element* element = checkElement(L);
	std::string attrName = luaL_checkstring(L, 2);
	std::string attrValue = luaL_checkstring(L, 3);
	element->setAttribute(attrName, attrValue);
	return 0;
}

int getSize(lua_State* L)
{
	Element* element = checkElement(L);
	Vector2 size = element->getComputedSize();
	lua_pushinteger(L, size.getX());
	lua_pushinteger(L, size.getY());
	return 2;
}

int getAbsolutePosition(lua_State* L)
{
	Element* element = checkElement(L);
	Vector2 absolutePosition = element->getAbsolutePosition();
	lua_pushinteger(L, absolutePosition.getX());
	lua_pushinteger(L, absolutePosition.getY());
	return 2;
}

int getPosition(lua_State* L)
{
	Element* element = checkElement(L);
	Vector2 position = element->getAbsolutePosition();
	lua_pushinteger(L, position.getX());
	lua_pushinteger(L, position.getY());
	return 2;
}

int getByTagName(lua_State* L)
{
	std::string tag = luaL_checkstring(L, 1);
	std::set<Element*> elements = Relax::getElementsByTagName(tag);
	lua_createtable(L, elements.size(), 0);
	int i = 1;
	for (std::set<Element*>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		lua_pushinteger(L, i);
		newRef(L, *it);
		lua_rawset(L, -3);
		i++;
	}
	return 1;
}

int style(lua_State* L)
{
	luaL_checktype(L, 1, LUA_TTABLE);
	lua_pushnil(L);
	while (lua_next(L, 1) != 0)
	{
		std::string tag = luaL_checkstring(L, -2);
		std::set<Element*> elements = Relax::getElementsByTagName(tag);
		luaL_checktype(L, -1, LUA_TTABLE);
		lua_pushnil(L);
		while (lua_next(L, -2) != 0)
		{
			std::string attrName = luaL_checkstring(L, -2);
			std::string attrValue = luaL_checkstring(L, -1);
			for (std::set<Element*>::iterator it = elements.begin(); it != elements.end(); it++)
			{
				// TODO: optimize loading of lua code (1 per attribute instead of 1 per element per attribute)
				(*it)->setAttribute(attrName, attrValue);
			}
			lua_pop(L, 1);
		}
		lua_pop(L, 1);
	}
	return 0;
}

}
}
}


