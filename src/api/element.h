#ifndef RELAX_API_ELEMENT_H
#define RELAX_API_ELEMENT_H

#include <lua5.2/lua.hpp>
#include "../element.h"

namespace relax
{
namespace api
{
namespace element
{

void open(lua_State* L);

void newref(lua_State* L, Element* element);

int tostring(lua_State* L);
int gettag(lua_State* L);
int setattribute(lua_State* L);
int getsize(lua_State* L);
int getabsoluteposition(lua_State* L);
int getposition(lua_State* L);

/* element.* functions */
int getbytag(lua_State* L);
int style(lua_State* L);

}
}
}

#endif


