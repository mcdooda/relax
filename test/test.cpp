#include <iostream>
#include <SDL/SDL.h>
#include "../src/relax.h"

using namespace relax;

int main(int argc, char* argv[])
{
	try
	{
		Relax::init();
		
		Relax r;
		r.addXML(
			"<block size='600 600' padding='5 5 5 5' anchor='center center' color='255 0 0 255'>"
				"<block background='test/background.png repeat' />"
			"</block>"
		);
		
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


