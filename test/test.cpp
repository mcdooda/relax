#include <iostream>
#include <SDL/SDL.h>
#include "../src/relax.h"

using namespace relax;

int main(int argc, char* argv[])
{
	try
	{
		lua_State* L = luaL_newstate();
		luaL_openlibs(L);
		
		Relax::init(L);
		
		Relax r;
		r.addXMLFile("test/ui.xml");
		r.runScript("test/init.lua");
		
		r.update();
		
		while (!!r)
		{
			r.pumpEvents();
			
			if (r.isPressed(K(ESCAPE)))
				r.close();
			
			r.render();
		}
		
		Relax::quit();
		lua_close(L);
	}
	catch (Exception ex)
	{
		std::cerr << ex.getMessage() << std::endl;
	}
	return 0;
}


