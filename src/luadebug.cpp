#include <iostream>
#include "luadebug.h"

namespace relax
{
namespace luadebug
{

void printStack(lua_State* L)
{
	std::cout << "=========================" << std::endl;
	std::cout << "top = " << lua_gettop(L) << std::endl;
	for (int i = lua_gettop(L); i >= 1; i--)
	{
		int type = lua_type(L, i);
		std::cout << i << "\t" << lua_typename(L, type) << "\t...\t";
		switch (type)
		{
			case LUA_TNIL:
			std::cout << "nil";
			break;
			
			case LUA_TNUMBER:
			std::cout << lua_tonumber(L, i);
			break;
			
			case LUA_TBOOLEAN:
			std::cout << (lua_toboolean(L, i) ? "true" : "false");
			break;
			
			case LUA_TSTRING:
			std::cout << lua_tostring(L, i);
			break;
			
			case LUA_TTABLE:
			std::cout << "# = " << lua_rawlen(L, i);
			break;
			
			case LUA_TFUNCTION:
			break;
			
			case LUA_TUSERDATA:
			case LUA_TLIGHTUSERDATA:
			std::cout << lua_touserdata(L, i);
			break;
			
			case LUA_TTHREAD:
			break;
		}
		std::cout << std::endl;
	}
	std::cout << "=========================" << std::endl;
}

}
}


