#include "mouse.h"
#include "../relax.h"

namespace relax
{
namespace api
{
namespace mouse
{

static const struct luaL_Reg lib_f[] = {
    {"getPosition", getPosition},
    {NULL, NULL}
};

void open(lua_State* L)
{
    luaL_newlib(L, lib_f);
    lua_setglobal(L, "mouse");
}

int getPosition(lua_State* L)
{
    Vector2 mouse = Relax::getMouse();
    lua_pushinteger(L, mouse.getX());
    lua_pushinteger(L, mouse.getY());
    return 2;
}

}
}
}
