#ifndef RELAX_MARGIN_H
#define RELAX_MARGIN_H

#include "rectangle.h"

namespace relax
{

class Margin : public Rectangle
{
	public:
		Margin();
		Margin(float top, float left, float right, float bottom);
};

}

#endif


