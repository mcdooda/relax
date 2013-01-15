#include <iostream>
#include <SDL/SDL.h>
#include "../src/relax.h"

using namespace relax;

int main(int argc, char* argv[])
{
	try
	{
		Relax::init();
		
		Relax r(false, true);
		r.addXMLFile("test/ui.xml");
		
		r.update();
		
		while (!!r)
		{
			r.pumpEvents();
			
			if (r.isPressed(K(ESCAPE)))
				r.close();
			
			r.render();
		}
		
		Relax::quit();
	}
	catch (Exception ex)
	{
		std::cerr << ex.getMessage() << std::endl;
	}
	return 0;
}


