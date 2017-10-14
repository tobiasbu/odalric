
#include "Exception.h"

namespace Odalric 
{
	Exception::Exception()
	{
		_message = "Exception!";
	}
	Exception::Exception(const char * str)
		:
		_message(str)
	{

	}
	Exception::Exception(const std::string & str)
		:
		_message(str.c_str())
	{

	}

	Exception::~Exception() throw ()
	{

	}
	const char* Exception::message() const throw()
	{
		return _message;
	}
}