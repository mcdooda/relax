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
					"<block size-x='200' anchor-x='center' color='0 255 0 255' />"
					"<block size-y='200' anchor-y='center' color='0 0 255 128' padding='10 10 10 10'>"
						"<block color-alpha='128'>Texte</block>"
					"</block>"
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


