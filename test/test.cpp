#include <iostream>
#include <SDL/SDL.h>
#include "../src/relax.h"

using namespace relax;

int main(int argc, char* argv[])
{
	try
	{
		Relax::init();
		
		Relax* r = new Relax();
		{
			Element* e1 = Element::fromXML(
				"<element1 width='400' height='400' padding='10 10 10 10' anchor-x='center'>"
					"<element2 width='300' color='0 255 0 255' />"
					"<element3 height='300' color='0 0 255 255' />"
				"</element1>"
			);
			r->addChild(e1);
		}
		
		r->update();
		
		while (!!*r)
		{
			char* keys = r->pumpEvents();
			if (keys[SDLK_ESCAPE])
				r->close();
			
			r->render();
		}
		
		delete r;
		Relax::quit();
	}
	catch (Exception ex)
	{
		std::cerr << ex.getMessage() << std::endl;
	}
	return 0;
}


