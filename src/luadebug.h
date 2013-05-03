#ifndef RELAX_LUADEBUG_H
#define RELAX_LUADEBUG_H

#include <lua5.2/lua.hpp>

namespace relax
{
namespace luadebug
{

void printStack(lua_State* L);

}
}

#endif


