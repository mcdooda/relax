#ifndef RELAX_PADDING_H
#define RELAX_PADDING_H

#include "rectangle.h"

namespace relax
{

class Padding : public Rectangle
{
	public:
		Padding();
		Padding(float top, float left, float right, float bottom);
};

}

#endif


