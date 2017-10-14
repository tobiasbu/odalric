
#ifndef _OAI_EXCEPTION_H_
#define _OAI_EXCEPTION_H_

#include <exception>
#include <string>

namespace Odalric {

	class Exception : public std::exception
	{
		const char * _message;

	public:

		Exception();
		Exception(const char * str);
		Exception(const std::string & str);
		~Exception() throw ();
		const char* message() const throw();
	};

}

#endif
