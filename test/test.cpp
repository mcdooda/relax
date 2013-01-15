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
		{
			Element* e1 = r.elementFromXML(
				"<block size='300 300' padding='5 5 5 5' anchor='center center'>"
					"<block background='test/background.png scale' />"
				"</block>"
			);
			r.addChild(e1);
		}
		
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


