#ifndef RELAX_EXCEPTION_H
#define RELAX_EXCEPTION_H

#include <string>

namespace relax
{

class Exception
{
	public:
		Exception();
		Exception(std::string message);
		
		inline const std::string& getMessage() { return m_message; }
		inline bool hasMessage() { return m_message != ""; }
		
	private:
		std::string m_message;
};

}

#endif


