#include "element.h"
#include "../element.h"

namespace relax
{
namespace api
{
namespace element
{

static const struct luaL_Reg lib_m[] = {
    {"__tostring",          tostring},
    {"gettag",              gettag},
    {"setattribute",        setattribute},
    {"getsize",             getsize},
    {"getabsoluteposition", getabsoluteposition},
    {"getposition",         getposition},
    {NULL, NULL}
};

void open(lua_State* L)
{
    luaL_newmetatable(L, "Relax.Element");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
    luaL_setfuncs(L, lib_m, 0);
}

int newref(lua_State* L, Element* element)
{
    Element** e = (Element**) lua_newuserdata(L, sizeof(Element*));
    luaL_getmetatable(L, "Relax.Element");
    lua_setmetatable(L, -2);
    *e = element;
    return 1;
}

#define checkElement(L) (*(Element**) luaL_checkudata(L, 1, "Relax.Element"))

int tostring(lua_State* L)
{
    Element* element = checkElement(L);
    lua_pushfstring(L, "<%s /> (%p)", element->getTag().c_str(), element);
    return 1;
}

int gettag(lua_State* L)
{
    Element* element = checkElement(L);
    lua_pushstring(L, element->getTag().c_str());
    return 1;
}

int setattribute(lua_State* L)
{
	Element* element = checkElement(L);
	std::string attrName = luaL_checkstring(L, 2);
	std::string attrValue = luaL_checkstring(L, 3);
	element->setAttribute(attrName, attrValue);
	return 0;
}

int getsize(lua_State* L)
{
	Element* element = checkElement(L);
	Vector2 size = element->getComputedSize();
	lua_pushinteger(L, size.getX());
	lua_pushinteger(L, size.getY());
	return 2;
}

int getabsoluteposition(lua_State* L)
{
	Element* element = checkElement(L);
	Vector2 absolutePosition = element->getAbsolutePosition();
	lua_pushinteger(L, absolutePosition.getX());
	lua_pushinteger(L, absolutePosition.getY());
	return 2;
}

int getposition(lua_State* L)
{
	Element* element = checkElement(L);
	Vector2 position = element->getAbsolutePosition();
	lua_pushinteger(L, position.getX());
	lua_pushinteger(L, position.getY());
	return 2;
}

}
}
}


