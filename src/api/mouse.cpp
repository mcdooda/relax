#include "mouse.h"
#include "../relax.h"

namespace relax
{
namespace api
{
namespace mouse
{

static const struct luaL_Reg lib_f[] = {
    {"getposition", getposition},
    {NULL, NULL}
};

void open(lua_State* L)
{
    luaL_newlib(L, lib_f);
    lua_setglobal(L, "mouse");
}

int getposition(lua_State* L)
{
    Vector2 mouse = Relax::getMouse();
    lua_pushinteger(L, mouse.getX());
    lua_pushinteger(L, mouse.getY());
    return 2;
}

}
}
}
