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

void newRef(lua_State* L, Element* element);

int toString(lua_State* L);
int getText(lua_State* L);
int getTag(lua_State* L);
int setAttribute(lua_State* L);
int getSize(lua_State* L);
int getAbsolutePosition(lua_State* L);
int getPosition(lua_State* L);

/* element.* functions */
int getByTagName(lua_State* L);
int style(lua_State* L);

}
}
}

#endif


