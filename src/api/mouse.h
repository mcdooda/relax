#ifndef RELAX_API_MOUSE_H
#define RELAX_API_MOUSE_H

#include <lua5.2/lua.hpp>

namespace relax
{
namespace api
{
namespace mouse
{

void open(lua_State* L);

int getPosition(lua_State* L);

}
}
}

#endif


