#include "exception.h"

namespace relax
{

Exception::Exception() :
	m_message("")
{
	
}

Exception::Exception(std::string message) :
	m_message(message)
{
	
}

}


